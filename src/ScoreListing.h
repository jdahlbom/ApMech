#ifndef AP_SCORELISTING_H
#define AP_SCORELISTING_H

#include <list>

#include "types.h"
#include "net/NetObject.h"
#include "net/Serializable.h"
#include "net/serializer.h"

namespace ap {

  class ScoreTuple : public ap::net::Serializable
  {
  public:
    ap::int32 uid;
    ap::uint16 kills;
    ap::uint16 deaths;
    ap::int16 score;

    int serialize(uint8 buffer[], int start, int buflength) const;
    int unserialize(uint8 buffer[], int start);
    void print() const;
  }; // struct ScoreTuple


  class ScoreListing : public ap::net::Serializable 
  {
  public:
    ScoreListing();
    ~ScoreListing();

    void addScore(ScoreTuple tuple, bool replace=false);
    void clearAllScores();
    bool getEachScore(ScoreTuple &result) const;

    int serialize(uint8 buffer[], int start, int buflength) const;
    int unserialize(uint8 buffer[], int start);

    void print();

  private:
    std::map<ap::int32, ScoreTuple> scoreList;
    mutable std::map<ap::int32, ScoreTuple>::const_iterator scoreIterator;

  }; // class ScoreListing

} // namespace ap


#endif
