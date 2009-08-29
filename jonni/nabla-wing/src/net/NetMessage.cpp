#include "NetMessage.h"

NetMessage::NetMessage(std::string _data, uint32 s, uint32 r)
{
    data = _data;
    sender = s;
    if (r == 0) recipients = NULL;
    else {
        recipients = new std::list<uint32>();
        recipients->push_back(r);
    }
}

NetMessage::NetMessage(std::string _data, uint32 s, std::list<uint32> &r)
{
    data = _data;
    sender = s;
    recipients = new std::list<uint32>();
    for (std::list<uint32>::iterator i = r.begin(); i != r.end(); i++)
    {
        recipients->push_back(*i);
    }
}

int NetMessage::serialize(ap::uint8 buffer[], int start, int buflength) const
{
    using ap::net::serialize;
    int length = 0;

    length += serialize(sender, buffer, start+length, buflength-length);
    if (!recipients) length += serialize(int(0), buffer, start+length, buflength-length);
    else {
        length += serialize(recipients->size(), buffer, start+length, buflength-length);
        for (std::list<uint32>::iterator i = recipients->begin(); i != recipients->end(); i++)
            length += serialize(*i, buffer, start+length, buflength-length);
    }
    strcpy( (char *)buffer + start+length, data.c_str());   length += data.length()+1;
    return length;
}

int NetMessage::unserialize(ap::uint8 buffer[], int start)
{
    using ap::net::unserialize;
    int length = 0, numrecipients;
    uint32 r;

    length += unserialize(sender, buffer, start+length);
    length += unserialize(numrecipients, buffer, start+length);

    if (numrecipients > 0)
    {
        recipients = new std::list<uint32>();
        for (int i=0; i<numrecipients; i++)
        {
            length += unserialize(r, buffer, start+length);
            recipients->push_back(r);
        }
    }

    data.assign((char *)buffer+start+length);           length += data.length()+1;
    return length;
}

