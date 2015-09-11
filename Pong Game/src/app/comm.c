/** @file   comm.c
 *  @author Sean O'Connor (sdo51)
 *  @author Matthew Edwards (mje109)
 *  @date   24 September 2012
 *
 *  The receiver is modeled as a state machine, with four modes as given in
 *  RecvPacketExpectation. The transmitter simply stores the packet being
 *  transmitted as a string along with the index of the current character.
 */


#include <stdio.h>
#include "system.h"
#include "ir_uart.h"

#include "comm.h"
#include "vector.h"
#include "game_state.h"
#include "debug.h"


/* Packet details */
/** @brief The identifiers for the four different packet types. */
const char packetIds[] = {'G', 'A', 'B', 'S'};

/** @brief The identifiers for the three different ball headings. */
const Vector headings[] = {{-1, -1}, {0, -1}, {1, -1}};

/** @brief Internal representation of a received packet. */
typedef struct
{
    /** @brief The packet type, as an index into packetIds. */
    PacketType type;

    /** @brief The data carried by the packet, or '\0\0'. */
    char data[2];
} Packet;


/* Send stuff */
/** @brief Buffer for packet currently being sent (global var). */
static char sending_packet[4];

/** @brief Index into sending_packet of the next character to send (global
 *  var). */
static uint8_t send_index;

/** @brief Flag for whether a packet is currently being sent (global var). */
static bool is_sending = FALSE;


/* Receive stuff */
/** @brief The four states the reciever can be in. */
typedef enum
{
    /** @brief The receiver is expecting PACKET_MAGIC. */
    PACKET_MAGIC_BYTE,

    /** @brief The receiver is expecting a packet type character. */
    PACKET_TYPE_BYTE,

    /** @brief The receiver is expecting the first data byte. */
    PACKET_DATA0_BYTE,

    /** @brief The receiver is expecting the second data byte. */
    PACKET_DATA1_BYTE
} RecvPacketExpectation;

/** @brief The packet currently being received (global var). */
static Packet recvingPacket = {0, {0, 0}};

/** @brief Flag for whether a packet has been successfully received (global var). */
static bool is_packet_ready = FALSE;


/* Functions */

/** @brief Translate a ball position from player co-ords to opponent co-ords */
static void flip_coords(Vector *ballPos) {
    ballPos->x = 6 - ballPos->x;
    ballPos->y = 9 - ballPos->y;
}


/** @brief Encode a ball position and heading into a char array for transmission */
static void serialise_ball(char *buf, const GameState *state) {
    Vector pos = state->ballPos;
    Vector heading = state->ballHeading;
    /* Translate to opponent co-ords */
    flip_coords(&pos);  
    heading.x *= -1;
    heading.y *= -1;
    /* Encode */
    ASSERT(0 <= pos.x && pos.x <= 9);
    ASSERT(pos.y == 5);
    buf[0] = '0' + pos.x;
    if (vector_equal(heading, headings[0])) {
        buf[1] = '0';
    }
    else if (vector_equal(heading, headings[1])) {
        buf[1] = '1';
    }
    else if (vector_equal(heading, headings[2])) {
        buf[1] = '2';
    }
    else {
        TRACE("Invalid ball heading.\n");
        buf[0] = buf[1] = 'x';
        return;
    }
}

/** @brief  Decode a received ball position */
static void deserialise_ball(char *buf, GameState *state) {
    ASSERT('0' <= buf[0] && buf[0] <= '9');
    state->ballPos.x = buf[0] - '0';
    state->ballPos.y = 5;
    ASSERT('0' <= buf[1] && buf[1] <= '2');
    state->ballHeading = headings[buf[1]-'0'];
}


/** @brief  Encode a game score into a char array for transmission */
static void serialise_score(char *buf, GameScore score) {
    if (score.player <= 9 && score.opponent <= 9) {
        /* Scores are stored as uints, so they are always positive */
        buf[0] = '0' + score.opponent;
        buf[1] = '0' + score.player;
    }
}

/** @brief  Decode a received score */
static void deserialise_score(char *buf, GameScore *score) {
    /* Assume the buffer contains valid data */
    score->player = buf[0] - '0';
    score->opponent = buf[1] - '0';
}


/** @brief  Encode a packet into a char array for transmission */
static void serialise_packet(char *buf, Packet packet) {
    buf[0] = PACKET_MAGIC;
    buf[1] = packetIds[packet.type];
    buf[2] = packet.data[0];
    buf[3] = packet.data[1];
}


/** @brief  Transmit a packet */
static void send_packet(Packet packet) {
    serialise_packet(sending_packet, packet);
    send_index = 0;
    is_sending = TRUE;
}


void comm_init(void) {
    ir_uart_init();
}


bool comm_is_sending(void) {
    return is_sending;
}


void comm_send_block(void) {
    while (comm_is_sending()) {
        comm_attempt_send();
    }
}


void comm_attempt_send(void) {
    if (is_sending && ir_uart_write_ready_p()) {
        ir_uart_putc(sending_packet[send_index]);
        if (ir_uart_read_ready_p()) {
            ir_uart_getc();  // Workaround for UCFK bug
        }
        send_index += 1;
        if (send_index > 3 || sending_packet[send_index] == 0) {
            is_sending = FALSE;
        }
    }
}


void comm_send_game_request(void) {
    Packet packet = {.type = GAME_REQUEST_PACKET, .data = {0, 0}};
    send_packet(packet);
}


void comm_send_ack(void) {
    Packet packet = {.type = ACK_PACKET, .data = {0, 0}};
    send_packet(packet);
}


void comm_send_ball(const GameState *state) {
    Packet packet = {.type = BALL_PACKET};
    serialise_ball(packet.data, state);
    send_packet(packet);
}


void comm_send_score(const GameScore score) {
    Packet packet = {.type = SCORE_PACKET};
    serialise_score(packet.data, score);
    send_packet(packet);
}


void comm_attempt_recv(void) {
    static RecvPacketExpectation nextRecvByte = PACKET_MAGIC_BYTE;
    if (ir_uart_read_ready_p()) {
        char c = ir_uart_getc();
        switch (nextRecvByte) {
            case PACKET_MAGIC_BYTE:
                if (c == PACKET_MAGIC) {
                    nextRecvByte = PACKET_TYPE_BYTE;
                    /* If there is a packet in the buffer, drop it. */
                    is_packet_ready = FALSE;
                }
                else {
                    TRACE("Invalid packet magic, dropping.\n");
                    is_packet_ready = FALSE;
                }
                break;
            case PACKET_TYPE_BYTE:
                if (c == packetIds[GAME_REQUEST_PACKET]) {
                    recvingPacket.type = GAME_REQUEST_PACKET;
                    is_packet_ready = TRUE;
                    nextRecvByte = PACKET_MAGIC_BYTE;
                }
                else if (c == packetIds[ACK_PACKET]) {
                    recvingPacket.type = ACK_PACKET;
                    is_packet_ready = TRUE;
                    nextRecvByte = PACKET_MAGIC_BYTE;
                }
                else if (c == packetIds[BALL_PACKET]) {
                    recvingPacket.type = BALL_PACKET;
                    nextRecvByte = PACKET_DATA0_BYTE;
                    is_packet_ready = FALSE;
                }
                else if (c == packetIds[SCORE_PACKET]) {
                    recvingPacket.type = SCORE_PACKET;
                    nextRecvByte = PACKET_DATA0_BYTE;
                    is_packet_ready = FALSE;
                }
                else {
                    TRACE("Invalid packet type, dropping.\n");
                    nextRecvByte = PACKET_MAGIC_BYTE;
                    is_packet_ready = FALSE;
                }
                break;
            case PACKET_DATA0_BYTE:
                if (c >= '0' && c <= '9') {
                    recvingPacket.data[0] = c;
                    nextRecvByte = PACKET_DATA1_BYTE;
                    is_packet_ready = FALSE;
                }
                else {
                    TRACE("Invalid packet data0, dropping.\n");
                    nextRecvByte = PACKET_MAGIC_BYTE;
                    is_packet_ready = FALSE;
                }
                break;
            case PACKET_DATA1_BYTE:
                if (c >= '0' && c <= '9') {
                    recvingPacket.data[1] = c;
                    is_packet_ready = TRUE;
                    nextRecvByte = PACKET_MAGIC_BYTE;
                }
                else {
                    TRACE("Invalid packet data1, dropping.\n");
                    nextRecvByte = PACKET_MAGIC_BYTE;
                    is_packet_ready = FALSE;
                }
                    
                break;
        }
    }
}


bool comm_is_packet_ready(PacketType type) {
    return (is_packet_ready && recvingPacket.type == type);
}


void comm_clear_packet(void) {
    is_packet_ready = FALSE;
}


void comm_recv_ball(GameState *state) {
    if (is_packet_ready && recvingPacket.type == BALL_PACKET) {
        deserialise_ball(recvingPacket.data, state);
    }
}


void comm_recv_score(GameScore *score) {
    if (is_packet_ready && recvingPacket.type == SCORE_PACKET) {
        deserialise_score(recvingPacket.data, score);
    }
}
    
