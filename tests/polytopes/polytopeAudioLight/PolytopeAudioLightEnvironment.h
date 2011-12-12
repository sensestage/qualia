/*
 * DataNetworkEnvironment.h
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

#ifndef DATANETWORKENVIRONMENT_H_
#define DATANETWORKENVIRONMENT_H_

/// this is the only file we need to include to interact with the datanetwork
#include "datanetwork.h"

#include "Environment.h"

/// this is the namespace used by the datanetwork classes
using namespace SWDataNetwork;

class DataNetworkEnvironment : public Environment {
public:
  Observation currentObservation;
  // should change to RLObservation later

  DataNetworkEnvironment(const char *hostip, const char *myport, const char *myName, int audioid, int lightid, int settingsid, int outputid ); // give input pars:
  // light sense node, audio sense node
  // output node
  // host ip, port, name

  virtual void init();
  virtual Observation* start();
  virtual Observation* step(const Action* action);

//  static void updateInput();

private:
  DataNetwork * dn;
  
  DataNode * audioNode;
  DataNode * lightNode;
  
  DataNode * settingsNode;
  DataNode * outNode;
  
  int audioid, lightid, settingsid, outputid;

};

#endif /* DATANETWORKENVIRONMENT_H_ */
