#include "NetMessage.h"

NetMessage::NetMessage(std::string _data, uint32 s, uint32 r)
{
    data.assign(_data);
    sender = s;
    if (r == 0) recipients = NULL;
    else {
        recipients = new std::list<uint32>();
        recipients->push_back(r);
    }
}

NetMessage::NetMessage(std::string _data, uint32 s, std::list<uint32> &r)
{
    data.assign(_data);
    sender = s;
    recipients = new std::list<uint32>();
    for (std::list<uint32>::iterator i = r.begin(); i != r.end(); i++)
    {
        recipients->push_back(*i);
    }
}

NetMessage::NetMessage(NetMessage &m)
{
    sender = m.sender;  data.assign(m.data);
    if (m.recipients) {
        recipients = new std::list<uint32>();
        for (std::list<uint32>::iterator i = m.recipients->begin(); i != m.recipients->end(); i++)
            recipients->push_back(*i);
    } else recipients = NULL;
}

void NetMessage::operator=(const NetMessage &m)
{
    sender = m.sender;  data.assign(m.data);
    if (recipients) delete recipients;  recipients = NULL;
    if (m.recipients) {
        recipients = new std::list<uint32>();
        for (std::list<uint32>::iterator i = m.recipients->begin(); i != m.recipients->end(); i++)
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
    cout << "msg = \""<<data.c_str()<<"\""<<endl;
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
    } else {
        if (recipients) delete recipients;
        recipients = NULL;
    }

    data.assign((char *)buffer+start+length);           length += data.length()+1;
    return length;
}

NetMessage::~NetMessage()
{
    // TODO: WTF should be here?
    if (recipients) {
        recipients->clear();
        delete recipients;
    }
}
