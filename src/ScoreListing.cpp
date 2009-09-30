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
    scoreList(std::map<ap::int32, ScoreTuple>()),
    scoreIterator(scoreList.begin()) {}

  ScoreListing::~ScoreListing() {}

  /**
   * Adds or replaces a tuple in the container. tuple.uid is used as unique key.
   *
   * @param ScoreTuple tuple   The tuple to be added to the container.
   * @param bool       replace If true, existing tuple with same uid will be replaced. Otherwise their values will be added together.
   */
  void ScoreListing::addScore(ScoreTuple tuple, bool replace) {
    std::map<ap::int32, ScoreTuple>::iterator it = scoreList.find(tuple.uid);

    if(it != scoreList.end()) {
      if (replace) {
	scoreList.erase(it->first);
      } else {
	it->second.kills += tuple.kills;
	it->second.deaths += tuple.deaths;
	it->second.score += tuple.score;
	return;
      }
    }
    scoreList.insert(std::make_pair<ap::int32, ScoreTuple>(tuple.uid, tuple));
    scoreIterator = scoreList.begin();
  }

  void ScoreListing::clearAllScores() {
    scoreList.clear();
    scoreIterator = scoreList.begin();
  }

  /**
   * Meant for iterating through every ScoreTuple in the container.
   *
   * Usage:  while(getEachScore(newTupleValues)) newTupleValues.print();
   *
   * @param ScoreTuple& result The values of next iterator position will be stored in this ScoreTuple.
   * @returns bool  True if container had a new element to show. False if nothing more to show.
   */
  bool ScoreListing::getEachScore(ScoreTuple &result) const {
    if (scoreList.end() != scoreIterator) {
      result.uid = scoreIterator->second.uid;
      result.kills = scoreIterator->second.kills;
      result.deaths = scoreIterator->second.deaths;
      result.score = scoreIterator->second.score;
      ++scoreIterator;
      return true;
    } else {
      scoreIterator = scoreList.begin();
      return false;
    }

    return false;
  }

  int ScoreListing::serialize(uint8 buffer[], int start, int buflength) const
  {
    int length = 0;
    ap::uint16 numberOfTuples = scoreList.size();
    
    length += ap::net::serialize(numberOfTuples, buffer, start+length, buflength-length);

    std::map<ap::int32, ScoreTuple>::const_iterator it = scoreList.begin();
    for (; it != scoreList.end(); ++it) {
      length += it->second.serialize(buffer, start+length, buflength+length);
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
    std::map<ap::int32, ScoreTuple>::const_iterator it;
    for (it = scoreList.begin(); it != scoreList.end(); ++it) {
      it->second.print();
    }
  }
} // namespace ap
