#include "../location/IRsensor.h"

using namespace std;

int main(int argc, char ** argv){
  IRsensor leftIR(0,"../location/left_IR_calibration.txt");
  IRsensor rightIR(1,"../location/right_IR_calibration.txt");
  ofstream left_file, right_file;
  left_file.open("../location/left_IR_calibration.txt");
  right_file.open("../location/right_IR_calibration.txt");
  double left_value, right_value;


  for (int i = 0; i < 142; i++){
  	left_value = leftIR.getValue();
  	right_value = rightIR.getValue();

    cout<< "Sample " << i << endl;
    cout << "----------------------------------------------" << endl;
    cout << "Left value: " <<  left_value  << endl; 
    cout << "Right value: " <<  right_value << endl; 
    cout << "----------------------------------------------" << endl << endl;

    left_file << left_value << endl;
    right_file << right_value << endl;
    sleep(3);
    }
  left_file.close();
  right_file.close();

  return 0;
}