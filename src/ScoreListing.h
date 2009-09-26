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
    ap::int16 kills;
    ap::int16 deaths;
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

    void addScore(ScoreTuple tuple);
    void clearAllScores();
    const ScoreTuple* getEachScore() const;

    int serialize(uint8 buffer[], int start, int buflength) const;
    int unserialize(uint8 buffer[], int start);

    void print();

  private:
    std::list<ScoreTuple> scoreList;
    mutable std::list<ScoreTuple>::const_iterator scoreIterator;
  }; // class ScoreListing

} // namespace ap


#endif
