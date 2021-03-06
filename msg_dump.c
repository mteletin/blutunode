/*
 * BluTuNode - Bluetooth sensor/actuator node software
 * Copyright (c) 2011-2012 Paul Sokolovsky
 *
 * BtNode is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software. If not, see <http://www.gnu.org/licenses/>.
 */

#include "btnode.h"

#ifdef DEBUG

#define MSG_DESC(msg_id, desc) {msg_id, #msg_id, desc}
static const struct MsgDescription {
    MessageId msg_id;
    char *name;
    char *desc;
} MSG_DESCRIPTIONS[] = {
    MSG_DESC(CL_INIT_CFM, "ConnectLib Initialized"),
    MSG_DESC(CL_RFCOMM_REGISTER_CFM, "RFCOMM Channel Allocated"),
/*    MSG_DESC(CL_DM_READ_BT_VERSION_CFM, ""),*/
    MSG_DESC(CL_DM_ACL_OPENED_IND, "ACL connection opened"),
    MSG_DESC(CL_DM_ACL_CLOSED_IND, "ACL connection closed"),
    MSG_DESC(CL_SM_PIN_CODE_IND, "Pin code request from a remote"),
    MSG_DESC(CL_SM_AUTHENTICATE_CFM, "Authentication result of remote device"),
    MSG_DESC(CL_SM_AUTHORISE_IND, "Authorize request for a remote trying to access service in security mode 2"),
    MSG_DESC(CL_RFCOMM_CONNECT_IND, "RFCOMM connection request from a remote"),
    MSG_DESC(CL_RFCOMM_CONNECT_CFM, "RFCOMM connection result"),
    MSG_DESC(MESSAGE_MORE_DATA, "More data available in source"),
    MSG_DESC(MESSAGE_MORE_SPACE, "More space available in sink"),
    MSG_DESC(MESSAGE_ADC_RESULT, "ADC reading available"),
    MSG_DESC(MESSAGE_PIO_CHANGED, "PIO pin(s) changed"),
    {0}
};

void dump_message(MessageId msg_id, Message msg)
{
    const struct MsgDescription *p;
    for (p = MSG_DESCRIPTIONS; p->msg_id; p++) {
        if (p->msg_id == msg_id) {
            PRINT(("Message: 0x%x %s (%s)\n", p->msg_id, p->name, p->desc));
            break;
        }
    }
    if (!p->msg_id) {
        PRINT(("Unknown message: 0x%x\n", msg_id));
        return;
    }
    
    switch (msg_id){
    case CL_INIT_CFM:
        {
            CAST_TYPED_MSG(CL_INIT_CFM, tmsg);
            print_status(tmsg->status);
            PRINT(("BT version code=%d\n", tmsg->version));
            break;
        }
    case CL_RFCOMM_REGISTER_CFM:
        {
            CAST_TYPED_MSG(CL_RFCOMM_REGISTER_CFM, tmsg);
            print_status(tmsg->status);
            PRINT(("RFCOMM channel=%d\n", tmsg->server_channel));
            break;
        }
    case CL_RFCOMM_CONNECT_IND:
        {
            CAST_TYPED_MSG(CL_RFCOMM_CONNECT_IND, tmsg);
            print_bdaddr(tmsg->bd_addr);
            PRINT(("RFCOMM channel=%d\n", tmsg->server_channel));
            PRINT(("Frame size=%d\n", tmsg->frame_size));
            break;
        }
    case CL_RFCOMM_CONNECT_CFM:
        {
            CAST_TYPED_MSG(CL_RFCOMM_CONNECT_CFM, tmsg);
            print_status(tmsg->status);
            PRINT(("RFCOMM channel=%d\n", tmsg->server_channel));
            PRINT(("Frame size=%d\n", tmsg->frame_size));
            PRINT(("Sink=%x\n", (int)tmsg->sink));
            break;
        }
    case CL_DM_ACL_OPENED_IND:
        {
            CAST_TYPED_MSG(CL_DM_ACL_OPENED_IND, tmsg);
            print_status(tmsg->status);
            PRINT(("Incoming=%d\n", tmsg->incoming));
            print_bdaddr(tmsg->bd_addr);
            break;
        }
    case CL_DM_ACL_CLOSED_IND:
        {
            CAST_TYPED_MSG(CL_DM_ACL_CLOSED_IND, tmsg);
            print_status(tmsg->status);
            print_bdaddr(tmsg->bd_addr);
            break;
        }
    case CL_SM_AUTHENTICATE_CFM:
        {
            CAST_TYPED_MSG(CL_SM_AUTHENTICATE_CFM, tmsg);
            print_status(tmsg->status);
            print_bdaddr(tmsg->bd_addr);
            PRINT(("Key type=%d\n", tmsg->key_type));
            PRINT(("Bonded=%d\n", tmsg->bonded));
            break;
        }
    case CL_SM_AUTHORISE_IND:
        {
            CAST_TYPED_MSG(CL_SM_AUTHORISE_IND, tmsg);
            print_bdaddr(tmsg->bd_addr);
            PRINT(("Incoming=%d\n", tmsg->incoming));
            PRINT(("Protocol=%d\n", tmsg->protocol_id));
            PRINT(("Channel=%ld\n", tmsg->channel));
            break;
        }
    case MESSAGE_MORE_DATA:
        {
            MessageMoreData *tmsg = (MessageMoreData*)msg;
            PRINT(("Source=%x\n", (int)tmsg->source));
            break;
        }
    case MESSAGE_MORE_SPACE:
        {
            MessageMoreSpace *tmsg = (MessageMoreSpace*)msg;
            PRINT(("Sink=%x\n", (int)tmsg->sink));
            break;
        }
    case MESSAGE_PIO_CHANGED:
        {
            MessagePioChanged *tmsg = (MessagePioChanged*)msg;
            PRINT(("GPIO state=%x\n", (int)tmsg->state));
            break;
        }
    }
}

#endif
