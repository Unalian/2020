/**************************************************************************
 
 File: Physics1.h
 
 Prepared by Erkin Tunca for nehe.gamedev.net
 
 **************************************************************************/

#include <math.h>
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
// class Vector3D		---> An object to represent a 3D vector or a 3D point in space
class Vector3D
{
public:
    float x;									// the x value of this Vector3D
    float y;									// the y value of this Vector3D
    float z;									// the z value of this Vector3D
    
    Vector3D()									// Constructor to set x = y = z = 0
    {
        x = 0;
        y = 0;
        z = 0;	}
    
    Vector3D(float x, float y, float z)			// Constructor that initializes this Vector3D to the intended values of x, y and z
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    
    Vector3D& operator= (Vector3D v)			// operator= sets values of v to this Vector3D. example: v1 = v2 means that values of v2 are set onto v1
    {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }
    
    Vector3D operator+ (Vector3D v)				// operator+ is used to add two Vector3D's. operator+ returns a new Vector3D
    {
        return Vector3D(x + v.x, y + v.y, z + v.z);
    }
    
    Vector3D operator- (Vector3D v)				// operator- is used to take difference of two Vector3D's. operator- returns a new Vector3D
    {
        return Vector3D(x - v.x, y - v.y, z - v.z);
    }
    
    Vector3D operator* (Vector3D v)			// operator* is used to scale a Vector3D by a value. This value multiplies the Vector3D's x, y and z.
    {
        return Vector3D(x * v.x, y * v.y, z * v.z);
    }
    
    Vector3D& operator+= (Vector3D v)			// operator+= is used to add another Vector3D to this Vector3D.
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    
    Vector3D& operator-= (Vector3D v)			// operator-= is used to subtract another Vector3D from this Vector3D.
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }
    
    
    Vector3D operator* (float value)			// operator* is used to scale a Vector3D by a value. This value multiplies the Vector3D's x, y and z.
    {
        return Vector3D(x * value, y * value, z * value);
    }
    
    Vector3D operator/ (float value)			// operator/ is used to scale a Vector3D by a value. This value divides the Vector3D's x, y and z.
    {
        return Vector3D(x / value, y / value, z / value);
    }
    
    Vector3D& operator*= (float value)			// operator*= is used to scale this Vector3D by a value.
    {
        x *= value;
        y *= value;
        z *= value;
        return *this;
    }
    
    Vector3D& operator/= (float value)			// operator/= is used to scale this Vector3D by a value.
    {
        x /= value;
        y /= value;
        z /= value;
        return *this;
    }
    
    
    Vector3D operator- ()						// operator- is used to set this Vector3D's x, y, and z to the negative of them.
    {
        return Vector3D(-x, -y, -z);
    }
    
    float length()								// length() returns the length of this Vector3D
    {
        return sqrtf(x*x + y*y + z*z);
    };
    
    void unitize()								// unitize() normalizes this Vector3D that its direction remains the same but its length is 1.
    {
        float length = this->length();
        
        if (length == 0)
            return;
        
        x /= length;
        y /= length;
        z /= length;
    }
    
    Vector3D unit()								// unit() returns a new Vector3D. The returned value is a unitized version of this Vector3D.
    {
        float length = this->length();
        
        if (length == 0)
            return *this;
        
        return Vector3D(x / length, y / length, z / length);
    }
    
};

// class Mass			---> An object to represent a mass
class Mass
{
public:
    float m;									// The mass value
    Vector3D pos;								// Position in space
    Vector3D vel;								// Velocity
    Vector3D force;								// Force applied on this mass at an instance
    
    Mass(float m)								// Constructor
    {
        this->m = m;
    }
    
    /*
     void applyForce(Vector3D force) method is used to add external force to the mass.
     At an instance in time, several sources of force might affect the mass. The vector sum
     of these forces make up the net force applied to the mass at the instance.
     */
    void applyForce(Vector3D force)
    {
        this->force += force;					// The external force is added to the force of the mass
    }
    
    /*
     void init() method sets the force values to zero
     */
    void init()
    {
        force.x = 0;
        force.y = 0;
        force.z = 0;
    }
    
    /*
     void simulate(float dt) method calculates the new velocity and new position of
     the mass according to change in time (dt). Here, a simulation method called
     "The Euler Method" is used. The Euler Method is not always accurate, but it is
     simple. It is suitable for most of physical simulations that we know in common
     computer and video games.
     */
    void simulate(float dt)
    {
        vel += (force / m) * dt;				// Change in velocity is added to the velocity.
        // The change is proportinal with the acceleration (force / m) and change in time
        
        pos += vel * dt;						// Change in position is added to the position.
        // Change in position is velocity times the change in time
    }
    
};


// class Simulation		---> A container object for simulating masses
class Simulation
{
public:
    int horinumOfMasses;
    int vertinumOfMasses;                            // number of masses in this container
    vector<vector<Mass*> > masses;									// masses are held by pointer to pointer. (Here Mass** represents a 1 dimensional array)
    
    Simulation(int horinumOfMasses, int vertinumOfMasses,float m)			// Constructor creates some masses with mass values m
    {
        vector<vector<Mass*> > arry;
        vector<Mass*> d;
        int i,j;
        this->horinumOfMasses = horinumOfMasses;
        this->vertinumOfMasses = vertinumOfMasses;
        for( i=0; i<vertinumOfMasses; i++ ){
            for( j=0; j<horinumOfMasses; j++ ){
                Mass* mass = new Mass(m);
                d.push_back(mass);
            }
            sort( d.begin(), d.end() );
            arry.push_back( d );
            d.resize(0);
        }
        masses = arry;
    }
    
    virtual void release()							// delete the masses created
    {
        for (int x = 0; x < vertinumOfMasses; ++x){
            for (int y = 0;y< horinumOfMasses;y++ )
            {
                delete(masses[x][y]);
                masses[x][y] = NULL;
            }
        }
        
        for (int x = 0; x < vertinumOfMasses; ++x){
            masses[x].clear();
        }
        masses.clear();
    }
    
    Mass* getMass(int indexx,int indexy)
    {
        if ((indexx <0 || indexy < 0 ) || indexx >= vertinumOfMasses || indexy >= horinumOfMasses)		// if the index is not in the array
            return NULL;							// then return NULL
        
        return masses[indexx][indexy];						// get the mass at the index
    }
    
    virtual void init()
    {   for (int x = 0; x < vertinumOfMasses; ++x){
            for (int y = 0;y< horinumOfMasses;++y)
            {
                masses[x][y]->init();
            }
        }
    }
    
    virtual void solve()							// no implementation because no forces are wanted in this basic container
    {
        // in advanced containers, this method will be overrided and some forces will act on masses
    }
    
    virtual void simulate(float dt)					// Iterate the masses by the change in time
    {   for (int x = 0; x < vertinumOfMasses; ++x){
            for (int y = 0;y< horinumOfMasses;++y)
            {
                masses[x][y]->simulate(dt);
            }
        }
    }
    
    virtual void operate(float dt)					// The complete procedure of simulation
    {
        init();										// Step 1: reset forces to zero
        solve();									// Step 2: apply forces
        simulate(dt);								// Step 3: iterate the masses by the change in time
    }
    
};

