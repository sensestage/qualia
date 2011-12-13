/*
 * PolytopeAudioEnvironment.h
 *
 * (c) 2011 Marije Baalman -- nescivi(@)gmail(.)com
 * (c) 2011 Sofian Audry -- info(@)sofianaudry(.)com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PolytopeAudioEnvIRONMENT_H_
#define PolytopeAudioEnvIRONMENT_H_

/// this is the only file we need to include to interact with the datanetwork
#include "datanetwork.h"

#include "Environment.h"
#include "RLObservation.h"

/// this is the namespace used by the datanetwork classes
using namespace SWDataNetwork;

class PolytopeAudioEnvironment : public Environment {
public:
  RLObservation currentObservation;
  // should change to RLObservation later

  PolytopeAudioEnvironment( DataNetwork * d, const char *myName, int audioid, int outputid ); // give input pars:
  // light sense node, audio sense node
  // output node
  // host ip, port, name

  void set_delay( float );

  virtual void init();
  virtual Observation* start();
  virtual Observation* step(const Action* action);

//  static void updateInput();

private:
  DataNetwork * dn;
  
  DataNode * audioNode;
  
//   DataNode * settingsNode;
  DataNode * outNode;
  
  int audioid, outputid;
  
  float delay;

};

#endif /* PolytopeAudioEnvIRONMENT_H_ */
