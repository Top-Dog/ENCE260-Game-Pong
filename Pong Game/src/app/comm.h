/** @file   comm.h
 *  @author Sean O'Connor (sdo51)
 *  @author Matthew Edwards (mje109)
 *  @date   24 September 2012
 *  @brief  IR communications module.
 *
 *  Packets have the following format:
 *  - 1 byte of magic (0x13)
 *  - 1 byte type field
 *  - Up to 2 bytes of data
 *  
 *  There are four packet types:
 *  - A game request request packet has type 'G' and no data.
 *  - An acknowledgement packet has type 'A' and no data.
 *  - A ball packet has type 'B', and two data bytes: the ball's x co-ordinate
 *    from the receiver's perspective, and the ball's heading from the
 *    receiver's perspective (where 0 is down and left, 1 is down, 2 is down and
 *    right).
 *  - A score packet has type 'S', and two data bytes: the receiver's score and
 *    the sender's score.
 *  All numbers are encoded as ASCII digits, to ease development.
 *
 *  It takes 4.16ms per byte to send a packet from start to finish. However,
 *  most of this time is spent waiting for the UART. Instead, by default all
 *  transmissions are done asynchronously, e.g.:
 *      @code
 *      pacer_init(500);
 *      comm_send_ack();
 *      while (comm_is_sending()) {
 *          comm_attempt_send();
 *          // do stuff
 *          pacer_wait();
 *      }
 *      @endcode
 *  Sending this way takes up almost no time and allows the main loop to execute
 *  without jitter. Note that depending on the pacer frequency, this can take
 *  dramatically longer altogether, but since the main loop is not blocking
 *  while the packet is being sent it isn't as important.
 *  
 *  Alternatively, the comm_send_block method will send a packet synchronously,
 *  e.g.:
 *      @code
 *      comm_send_ack();
 *      comm_send_block();
 *      @endcode
 */


#ifndef COMM_H
#define COMM_H

#include "vector.h"
#include "game_state.h"
#include "bool.h"


/** @brief Identifier for the start of a packet. */
#define PACKET_MAGIC 0x13


/** @brief Packet types. */
typedef enum
{
    /** @brief Game request packet, sent in SEARCHING mode.
     *
     *  data is unused. */
    GAME_REQUEST_PACKET,

    /** @brief Acknowledgment packet, sent in reply to a game request packet.
     *
     *  data is unused. */
    ACK_PACKET,

    /** @brief Ball packet, sent when the ball leaves the top of the screen.
     *
     *  data[0] is x position, data[1] is heading. */
    BALL_PACKET,

    /** @brief  Score packet, sent when the ball leaves the bottom of the screen.
     *
     *  data[0] is receiever's score, data[1] is sender's score. */
    SCORE_PACKET
} PacketType;


/** @brief Initialise the module */
void comm_init(void);


/* Send */

/** @brief  Return whether a packet is currently being sent */
bool comm_is_sending(void);

/** @brief  Block until the current packet is sent */
void comm_send_block(void);

/** @brief  If the transmitter is free, send the next byte of the current packet */
void comm_attempt_send(void);

/** @brief  Send a game request packet to the opponent */
void comm_send_game_request(void);

/** @brief  Send an acknowledgement packet to the opponent */
void comm_send_ack(void);

/** @brief  Send the ball position and heading to the opponent */
void comm_send_ball(const GameState *state);

/** @brief  Send the score to the opponent */
void comm_send_score(const GameScore score);


/* Receive */

/** @brief  Attempt to receive any recent packets */
void comm_attempt_recv(void);

/** @brief  Discard the most recently received packet and start accepting new packets */
void comm_clear_packet(void);

/** @brief  Check if there are any decoded packets available of the given type */
bool comm_is_packet_ready(PacketType type);

/** @brief  Read the ball position and heading sent by the opponent */
void comm_recv_ball(GameState *state);

/** @brief  Read the score sent by the opponent */
void comm_recv_score(GameScore *score);


#endif /* COMM_H */
