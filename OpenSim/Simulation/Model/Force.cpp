// Force.cpp
// Author: Peter Eastman, Ajay Seth
/*
 * Copyright (c) 2009 Stanford University. All rights reserved. 
* Use of the OpenSim software in source form is permitted provided that the following
* conditions are met:
* 	1. The software is used only for non-commercial research and education. It may not
*     be used in relation to any commercial activity.
* 	2. The software is not distributed or redistributed.  Software distribution is allowed 
*     only through https://simtk.org/home/opensim.
* 	3. Use of the OpenSim software or derivatives must be acknowledged in all publications,
*      presentations, or documents describing work in which OpenSim or derivatives are used.
* 	4. Credits to developers may not be removed from executables
*     created from modifications of the source.
* 	5. Modifications of source code must retain the above copyright notice, this list of
*     conditions and the following disclaimer. 
* 
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
*  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
*  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
*  SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
*  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
*  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
*  OR BUSINESS INTERRUPTION) OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
*  WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "Force.h"
#include "Model.h"

namespace OpenSim {

//=============================================================================
// CONSTRUCTOR(S) AND DESTRUCTOR
//=============================================================================
//_____________________________________________________________________________
/**
 * Default constructor.
 */
	Force::Force():ModelComponent() 
{
	setNull();
	setupProperties();
}

//_____________________________________________________________________________
/**
 * Destructor.
 */
Force::~Force()
{
}

//_____________________________________________________________________________
/**
 * Copy constructor.
 *
 * @param aForce Force to be copied.
 */
Force::Force(const Force &aForce) : ModelComponent(aForce)
{
	setNull();
	setupProperties();
	copyData(aForce);
}

//=============================================================================
// CONSTRUCTION METHODS
//=============================================================================
//_____________________________________________________________________________
/**
 * Copy data members from one Force to another.
 *
 * @param aForce Force to be copied.
 */
void Force::copyData(const Force &aForce)
{
	//_isDisabled = aForce._isDisabled;
	_isDisabledProp.setValue(aForce._isDisabledProp.getValueBool());
	_model = aForce._model;
	// A copy is no longer a live Force with an underlying SimTK::Force
	// The system must be created, at which time the Force will be assigned an index
	// corresponding to a valid system SimTK::Force.
	_index.invalidate();
}

//_____________________________________________________________________________
/**
 * Set the data members of this Force to their null values.
 */
void Force::setNull(void)
{
	setType("Force");
}

//_____________________________________________________________________________
/**
 * Connect properties to local pointers.
 */
void Force::setupProperties(void)
{
	_isDisabledProp.setName("isDisabled");
	_isDisabledProp.setValue(false);
	_propertySet.append(&_isDisabledProp);
}

void Force::initState(SimTK::State& s) const
{
	SimTK::Force& simForce = _model->updUserForceSubsystem().updForce(_index);

	// Otherwise we have to change the status of the constraint
	if(_isDisabledProp.getValueBool())
		simForce.disable(s);
	else
		simForce.enable(s);
}

void Force::setDefaultsFromState(const SimTK::State& state)
{
    _isDisabledProp.setValue(isDisabled(state));
}

//=============================================================================
// OPERATORS
//=============================================================================
//_____________________________________________________________________________
/**
 * Assignment operator.
 *
 * @return Reference to this object.
 */
Force& Force::operator=(const Force &aForce)
{
	// BASE CLASS
	Object::operator=(aForce);

	copyData(aForce);

	return(*this);
}


void Force::setup(Model& model)
{
	ModelComponent::setup(model);
}

//_____________________________________________________________________________
/**
 * Set whether or not this Force is disabled.
 * Simbody multibody system instance is realized every time the isDisabled
 * changes, BUT multiple sets to the same value have no cost.
 *
 * @param isDisabled If true the force is disabled; if false the Force is enabled.
 */
void Force::setDisabled(SimTK::State& s, bool isDisabled) 
{
	if(_index.isValid()){
		SimTK::Force& simtkForce = _model->updUserForceSubsystem().updForce(_index);
		if(isDisabled)
			simtkForce.disable(s);
		else
			simtkForce.enable(s);
	}
	_isDisabledProp.setValue(isDisabled);
}

bool Force::isDisabled(const SimTK::State& s) const
{
	if(_index.isValid()){
		SimTK::Force& simtkForce = _model->updUserForceSubsystem().updForce(_index);
		return simtkForce.isDisabled(s);
	}
	return _isDisabledProp.getValueBool();
}


} // end of namespace OpenSim
