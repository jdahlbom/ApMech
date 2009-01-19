#include "functions.h"

string uint2ipv4(unsigned int ip)
{
    stringstream out;
    out << (ip&255) <<"."<< ((ip>>8)&255) <<"."<< ((ip>>16)&255) <<"."<< (ip>>24);
    return out.str();
}


void hexprint(enet_uint8 *buffer, int length)
{
    enet_uint8 u;
    cout << "Buf length = "<<length<<": ";
    cout << "(";
    for (int i=0; i<length; i++) {
        u = buffer[i];

        if ((u > 64) && (u < 91)) cout << char(u);
        else if ((u > 96) && (u < 123)) cout << char(u);
        else cout << int(u);

        if (i != length-1) cout << ",";
    }

    cout << ")"<<endl;
}

template <class T> inline std::string to_string (const T &t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

long int getTicks()
{
    static long int startticks = -1;
    static time_t starttime_t;
    static timeval now;

    gettimeofday(&now, NULL);

    if (startticks != -1) { // then the function is initialized already
        return (now.tv_sec - starttime_t)*1000 + (now.tv_usec - startticks)*0.001;
    } else {
        starttime_t = now.tv_sec;
        startticks = now.tv_usec*0.001;
        return 0;
    }
}
