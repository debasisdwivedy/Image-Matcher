// B657 assignment 2 skeleton code
//
// Compile with: "make"
//
// See assignment handout for command line and project specifications.


//Link to the header file
#include "CImg.h"
#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <Sift.h>
#include <math.h>

//Use the cimg namespace to access the functions easily
using namespace cimg_library;
using namespace std;


int sift_matching(CImg<double> input1, CImg<double> input2, int MINIMUM_SIFT_DISTANCE  = 100)
{
    int matches=0, maxHeight = 0;
    const unsigned char color[] = {0, 255, 0};
    
    CImg<double> greyScale1, greyScale2;
    
    // converting the images to grayscale
    if(input1.spectrum() == 1)  
        greyScale1 = input1;
    else    
        greyScale1 = input1.get_RGBtoHSI().get_channel(2);

    if(input2.spectrum() == 1)
    	greyScale2 = input2;
    else
	greyScale2 = input2.get_RGBtoHSI().get_channel(2); 
    
    vector<SiftDescriptor> desc1 = Sift::compute_sift(greyScale1);
    vector<SiftDescriptor> desc2 = Sift::compute_sift(greyScale2);
    
    
    // creating a merged image using the two images
    maxHeight = max(input1.height(),input2.height());

    CImg<double> output(input1.width() + input2.width(), maxHeight, 1, input1.spectrum(), 0);

	// first image on the left of the merged image
	for(int i = 0; i < input1.width(); i++)
	{
		for(int j = 0; j < input1.height(); j++)
		{
		        for(int k = 0; k < input1.spectrum(); k++)
			{
			    output(i, j, 0, k) = input1(i, j, 0, k);                    
			}
		}
	}

	// second image on the right of the merged image
	for(int i = input1.width(); i < output.width(); i++)
	{
		for(int j = 0; j < input2.height(); j++)
		{
		        for(int k = 0; k < input2.spectrum(); k++)
			{
			    output(i, j, 0, k) = input2(i - input1.width(), j, 0, k);                    
			}
		}
	}
    
    
    // matching both the descriptors and obtaining the number of matches
    for(int i = 0; i < desc1.size(); i++)
    {
        for(int j = 0; j < desc2.size(); j++)
        {
            double sum = 0;

            for(int k = 0; k < 128; k++)
            {
                sum += pow((desc1[i].descriptor[k]-desc2[j].descriptor[k]),2);
            }
            
            sum = sqrt(sum);

            if(sum < MINIMUM_SIFT_DISTANCE)
            {
                output.draw_line(desc1[i].col, desc1[i].row, desc2[j].col + input1.width(), desc2[j].row, color);
                matches++;
            }
        }
    }
    
    output.save("sift.png");
    
    return matches; 
}


int main(int argc, char **argv)
{
  try {

    if(argc < 2)
      {
	cout << "Insufficent number of arguments; correct usage:" << endl;
	cout << "    a2-p1 part_id ..." << endl;
	return -1;
      }

    string part = argv[1];
    string inputFile = argv[2];

    // Sift detector
    if(part == "part1")
    {
	   if(argc < 4)
        {
            cout << "Insufficent number of arguments." << endl;
            return -1;
        }

        CImg<double> input1(argv[2]);
        CImg<double> input2(argv[3]);

        double matches=sift_matching(input1, input2);
        cout<<matches<<endl;            
    }
    else if(part == "part2")
      {
	// do something here!
      }
    else
      throw std::string("unknown part!");

    // feel free to add more conditions for other parts (e.g. more specific)
    //  parts, for debugging, etc.
  }
  catch(const string &err) {
    cerr << "Error: " << err << endl;
  }
}
