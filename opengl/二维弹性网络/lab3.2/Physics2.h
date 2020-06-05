/**************************************************************************
 
 File: Physics2.h
 
 Prepared by Erkin Tunca for nehe.gamedev.net
 
 **************************************************************************/

#include "Physics1.h"									//Physics1.h is a must for Physics2.h simulations

class Spring											//An object to represent a spring with inner friction binding two masses. The spring 
//has a normal length (the length that the spring does not exert any force)
{
public:
    Mass* mass1;										//The first mass at one tip of the spring
    Mass* mass2;										//The second mass at the other tip of the spring
    
    float springConstant;								//A constant to represent the stiffness of the spring
    float springLength;									//The length that the spring does not exert any force
    float frictionConstant;								//A constant to be used for the inner friction of the spring
    
    Spring(Mass* mass1, Mass* mass2,
           float springConstant, float springLength, float frictionConstant)		//Constructor
    {
        this->springConstant = springConstant;									//set the springConstant
        this->springLength = springLength;										//set the springLength
        this->frictionConstant = frictionConstant;								//set the frictionConstant
        
        this->mass1 = mass1;													//set mass1
        this->mass2 = mass2;													//set mass2
    }
    
    void solve()																	//solve() method: the method where forces can be applied
    {
        Vector3D springVector = mass1->pos - mass2->pos;							//vector between the two masses
        
        float r = springVector.length();											//distance between the two masses
        
        Vector3D e, force;															//force initially has a zero value
        
        if (r != 0)																	//to avoid a division by zero check if r is zero
        {
            e = springVector / r;
            force += e * (r - springLength) * (-springConstant);	//the spring force is added to the force
            
            force += -e * (mass1->vel*e - mass2->vel*e) * frictionConstant;						//the friction force is added to the force
            //with this addition we obtain the net force of the spring
        }
        mass1->applyForce(force);													//force is applied to mass1
        mass2->applyForce(-force);													//the opposite of force is applied to mass2
    }
    
};

/*
 class RopeSimulation is derived from class Simulation (see Physics1.h). It simulates a rope with
 point-like particles binded with springs. The springs have inner friction and normal length. One tip of
 the rope is stabilized at a point in space called "Vector3D ropeConnectionPos". This point can be
 moved externally by a method "void setRopeConnectionVel(Vector3D ropeConnectionVel)". RopeSimulation
 creates air friction and a planer surface (or ground) with a normal in +y direction. RopeSimulation
 implements the force applied by this surface. In the code, the surface is refered as "ground".
 */
class RopeSimulation :  public Simulation				//An object to simulate a rope interacting with a planer surface and air
{
public:
    int horinumOfMasses;
    int vertinumOfMasses;
    vector<vector<Spring*>>vertiSprings;
    vector<vector<Spring*>> horiSprings;
    //Springs binding the masses (there shall be [numOfMasses - 1] of them)
    
    Vector3D gravitation;								//gravitational acceleration (gravity will be applied to all masses)
    
    Vector3D ropeConnectionPos;							//A point in space that is used to set the position of the
    //first mass in the system (mass with index 0)
    
    Vector3D ropeConnectionVel;							//a variable to move the ropeConnectionPos (by this, we can swing the rope)
    
    RopeSimulation(										//a long long constructor with 11 parameters starts here
                   int horinumOfMasses,
                   int vertinumOfMasses,                        //1. the number of masses
                   float m,										//2. weight of each mass
                   float springConstant,							//3. how stiff the springs are
                   float springLength,								//4. the length that a spring  does not exert any force
                   float springFrictionConstant,					//5. inner friction constant of spring
                   Vector3D gravitation							//6. gravitational acceleration
    
    ) : Simulation(horinumOfMasses,vertinumOfMasses, m)					//The super class creates masses with weights m of each
    {
        this->gravitation = gravitation;
        this->horinumOfMasses = horinumOfMasses;
        this->vertinumOfMasses = vertinumOfMasses;
        
 
        for (int x = 0; x < vertinumOfMasses; x++){
            for (int y = 0;y< horinumOfMasses;y++ )
            {
                masses[x][y]->pos.x = y * springLength;        //Set x position of masses[a] with springLength distance to its neighbor
                masses[x][y]->pos.y = -x * springLength;                        //Set y position as 0 so that it stand horizontal with respect to the ground
                masses[x][y]->pos.z = 0;                        //Set z position as 0 so that it looks simple
            }
        }
        
        vector<vector<Spring*> > arry;
        vector<Spring*> d;
      
        
        for( int x=0; x<vertinumOfMasses-1; x++ ){
            for( int y=0; y<horinumOfMasses; y++){
                Spring* spring = new Spring(masses[x][y], masses[x+1][y],
                springConstant, springLength, springFrictionConstant);
                d.push_back(spring);
            }
            sort( d.begin(), d.end() );
            arry.push_back( d );
            d.resize(0);
        }
        this->horiSprings = arry;
        
        
        vector<vector<Spring*> > arry1;
        vector<Spring*> d1;
        for( int x=0; x<vertinumOfMasses; x++ ){
            for( int y=0; y<horinumOfMasses-1; y++){
                Spring* spring = new Spring(masses[x][y], masses[x][y+1],
                springConstant, springLength, springFrictionConstant);
                d1.push_back(spring);
            }
            sort( d1.begin(), d1.end() );
            arry1.push_back( d1 );
            d.resize(0);
        }
        this->vertiSprings = arry1;
        
    }
    
    void release()										//release() is overriden because we have springs to delete
    {
        Simulation::release();							//Have the super class release itself
        
        for( int x=0; x<vertinumOfMasses-1; x++ ){
            for( int y=0; y<horinumOfMasses; y++){
                delete(horiSprings[x][y]);
                horiSprings[x][y] = NULL;
            }
        }
        for( int x=0; x<vertinumOfMasses-1; x++ ){
            horiSprings[x].clear();
        }
        horiSprings.clear();
        
        for( int x=0; x<vertinumOfMasses; x++ ){
            for( int y=0; y<horinumOfMasses-1; y++){
                delete(vertiSprings[x][y]);
                vertiSprings[x][y] = NULL;
            }
        }
        for( int x=0; x<vertinumOfMasses; x++ ){
            vertiSprings[x].clear();
        }
        vertiSprings.clear();
    }
    
    void solve()										//solve() is overriden because we have forces to be applied
    {
        for( int x=0; x<vertinumOfMasses-1; x++ ){
            for( int y=0; y<horinumOfMasses; y++){
                horiSprings[x][y] ->solve();
            }
        }
        for( int x=0; x<vertinumOfMasses; x++ ){
            for( int y=0; y<horinumOfMasses-1; y++){
                vertiSprings[x][y] ->solve();
            }
        }
  
    }
    
    void simulate(float dt)								//simulate(float dt) is overriden because we want to simulate
    //the motion of the ropeConnectionPos
    {
        Simulation::simulate(dt);						//the super class shall simulate the masses
        
        ropeConnectionPos += ropeConnectionVel * dt;	//iterate the positon of ropeConnectionPos
        
        masses[0][0]->pos = ropeConnectionPos;				//mass with index "0" shall position at ropeConnectionPos
        masses[0][0]->vel = ropeConnectionVel;				//the mass's velocity is set to be equal to ropeConnectionVel
    }
    
    void setRopeConnectionVel(Vector3D ropeConnectionVel)	//the method to set ropeConnectionVel
    {
        this->ropeConnectionVel = ropeConnectionVel;
    }
    
};
