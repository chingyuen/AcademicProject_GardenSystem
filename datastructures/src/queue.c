/* queue.c --- 
 * 
 * Filename: queue.c
 * Description: 
 * Author: Brian Rak
 * Maintainer: 
 * Created: Thu Feb  7 19:49:26 2013 (-0500)
 * Version: 
 * Last-Updated: 
 *           By: 
 *     Update #: 0
 * URL: 
 * Doc URL: 
 * Keywords: 
 * Compatibility: 
 * 
 */

/* Commentary: 
 * 
 * 
 * 
 */

/* Change Log:
 * 
 * 
 */

/* This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth
 * Floor, Boston, MA 02110-1301, USA.
 */

/* Code: */

#include <queue.h>

void init_queue(queue_t *buf) {
    buf->head = buf->tail = 0;
}
// returns 1 if success, 0 if fail
int enqueue (queue_t *buf, int data) {
    if ((buf->tail + 1) % QUEUE_SIZE == buf->head)
        return 0;
    buf->tail = ++buf->tail % QUEUE_SIZE;
    buf->buffer[buf->tail] = data;
    return 1;
}

// if the buffer is empty 0 should be returned, else 1
int dequeue (queue_t *buf) {
    if (buf->tail == buf->head)
        return 0;
    buf->head = ++buf->head % QUEUE_SIZE;
    return buf->buffer[buf->head];
}

// return 1 if empty, 0 if not
int queue_empty(queue_t *buf) {
    return buf->head == buf->tail ? 1 : 0;
}
