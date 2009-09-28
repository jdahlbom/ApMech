#include "ScoreListing.h"

#include "net/serializer.h"

namespace ap {

  int ScoreTuple::serialize(uint8 buffer[], int start, int buflength) const
  {
    int length = 0;
    length += ap::net::serialize(uid, buffer, start+length, buflength-length);
    length += ap::net::serialize(kills, buffer, start+length, buflength-length);
    length += ap::net::serialize(deaths, buffer, start+length, buflength-length);
    length += ap::net::serialize(score, buffer, start+length, buflength-length);
    return length;
  }

  int ScoreTuple::unserialize(uint8 buffer[], int start)
  {
    int length = 0;
    length += ap::net::unserialize(uid, buffer, start+length);
    length += ap::net::unserialize(kills, buffer, start+length);
    length += ap::net::unserialize(deaths, buffer, start+length);
    length += ap::net::unserialize(score, buffer, start+length);
    return length;
  }

  void ScoreTuple::print() const
  {
    std::cout << "uid: " << uid 
	      <<", kills: " << kills
	      <<", deaths: " << deaths
	      <<", score: " << score 
	      <<std::endl;
  }

  ScoreListing::ScoreListing() :
    scoreList(std::list<ScoreTuple>()),
    scoreIterator(scoreList.end()) {}

  ScoreListing::~ScoreListing() {}

  void ScoreListing::addScore(ScoreTuple tuple) {
    // TODO: Should this ensure there are no duplicate uids?
    scoreList.push_back(tuple);
  }

  void ScoreListing::clearAllScores() {
    scoreList.clear();
    scoreIterator = scoreList.end();
  }

  const ScoreTuple* ScoreListing::getEachScore() const {
    if (scoreList.end() == scoreIterator) {
      scoreIterator = scoreList.begin();
    } else {
      ++scoreIterator;
    }

    if (scoreIterator != scoreList.end()) {
      return &(*scoreIterator);
    }
    return 0;
  }

  int ScoreListing::serialize(uint8 buffer[], int start, int buflength) const
  {
    int length = 0;
    ap::uint16 numberOfTuples = scoreList.size();
    
    length += ap::net::serialize(numberOfTuples, buffer, start+length, buflength-length);

    std::list<ScoreTuple>::const_iterator it = scoreList.begin();
    for (; it != scoreList.end(); ++it) {
      length += (*it).serialize(buffer, start+length, buflength+length);
    }
    return length;
  }

  int ScoreListing::unserialize(uint8 buffer[], int start) 
  {
    int length = 0;
    ap::uint16 numberOfTuples = 0;
    length += ap::net::unserialize(numberOfTuples, buffer, start+length);
    
    scoreList.clear();
    ScoreTuple tempTuple;
    for (int i=0; i<numberOfTuples; ++i) {
      length += tempTuple.unserialize(buffer, start+length);
      addScore(tempTuple);
    }
    return length;
  }

  void ScoreListing::print()
  {
    std::list<ScoreTuple>::const_iterator it = scoreList.begin();
    for (; it != scoreList.end(); ++it) {
      (*it).print();
    }
  }
} // namespace ap
