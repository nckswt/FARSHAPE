
//Behaviours are atomic commands that each robot can perform. A behaviour list is a sequential form of Behaviours
class Behaviours{
public:
	Behaviours();
	~Behaviours();

	//Atomic Behaviours
	void setupActuatorsAndSensors();
	void findTarget();
	void moveToTarget();
	void grabTarget();

	//Behaviour Lists
	void *trackObject();
};