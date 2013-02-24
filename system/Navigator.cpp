Navigator::Navigator(){

}

Navigator::~Navigator(){

}

void Navigator::identifyTarget(string){
	//open filename
	//get x, z coordinates
	//close filename

	//read IR sensor
	//get y coordinate

	//set self.targetPosition
	//<>TODO: REMOVE THIS STUB----------------------------
	double randx = randint(3,10);
	double randy = randint(3,10);
	double randz = randint(0,3);
	Cart3D STUBPOSITION = {randx, randy, randz};
	STUBSPHERICAL = rectangular2spherical(STUBPOSITION);
	this.targetPosition = STUBSPHERICAL;
	//----------------------------------------------------
}

Cart3D* Navigator::identifyObjects(){

}

Cart3D* Navigator::mapObjectPositions(){

}

Cart3D Navigator::mapStructurePosition(){

}

SphericalCoordinate Navigator::rect2spherical(Cart3D){
	SphericalCoordinate spherical;
    //r is distance to object
    spherical.r = math.sqrt(x^2 + y^2 + z^2);
    //theta is lateral angle
    spherical.theta = math.degrees(math.acos(z/r)); 
    //phi is elevation angle
    spherical.phi = math.degrees(math.atan(y/x));
    return spherical
}