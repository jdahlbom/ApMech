#ifndef AP_SCORELISTING_H
#define AP_SCORELISTING_H

#include "net/NetObject.h"

#include <map>

#include "types.h"
#include "net/Serializable.h"
#include "net/serializer.h"

namespace ap {

  class ScoreTuple : public ap::net::Serializable
  {
  public:
    ap::uint32 uid;
    ap::uint16 kills;
    ap::uint16 deaths;
    ap::int16 score;

    int serialize(uint8 buffer[], int start, int buflength) const;
    int unserialize(uint8 buffer[], int start);
    void print() const;
  }; // struct ScoreTuple


  class ScoreListing : public ap::net::NetObject
  {
  public:
    ScoreListing();
    ~ScoreListing();

    void addScore(const ScoreTuple& tuple, bool replace=false);
    void removeScore(ap::uint32 id);
    void clearAllScores();
    bool getEachScore(ScoreTuple &result) const;
    void print() const;

    // interface inherited from NetObject
    uint8 getObjectType() const;
    NetObject *create(ap::uint32 id) const;
    int serialize(uint8 buffer[], int start, int buflength) const;
    int unserialize(uint8 buffer[], int start);


  private:
    std::map<ap::uint32, ScoreTuple> scoreList;
    mutable std::map<ap::uint32, ScoreTuple>::const_iterator scoreIterator;

  }; // class ScoreListing

} // namespace ap


#endif
