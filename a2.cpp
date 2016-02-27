// B657 assignment 2 skeleton code
//
// Compile with: "make"
//
// See assignment handout for command line and project specifications.


//Link to the header file
#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <math.h>
#include "CImg.h"
#include "Sift.h"
#include "Matrix.h"
#include "Transform.h"
#include <map>
#include <list>

//Use the cimg namespace to access the functions easily
using namespace cimg_library;
using namespace std;

// holds the name of the image file and the matching score of sift descriptors
class Image
{
    public:

    string name;
    int count;

    void setParameters(string name, int count)
    {
        this->name = name;
        this->count = count;
    }

    int getCount()
    {
        return count;
    }

    string getName()
    {
        return name;
    }

    bool operator < (const Image& i1) const
    {
        return count > i1.count;
    }
};

int sift_matching(CImg<double> input1, CImg<double> input2, string part, int MINIMUM_SIFT_DISTANCE  = 100)
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
	if(part == "part1.1")
    {
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
                if(part == "part1.1")
                {
                    output.draw_line(desc1[i].col, desc1[i].row, desc2[j].col + input1.width(), desc2[j].row, color);
                }
                matches++;
            }
        }
    }

    // saving the sift output to a image file
    if(part == "part1.1")
    {
        output.save("sift.png");
    }

    return matches;
}

void img_display(const CImg<double>& img) {
	cimg_library::CImgDisplay main_disp(img, "temp");
	while (!main_disp.is_closed() )
		main_disp.wait();

}

int temp() {
#if 0
	CImg<double> img("lincoln.png");
	SqMatrix mat = SqMatrix::identity(3);

	Transformation t(mat);
	t.translate(30,30);
	t.rotate(-M_PI/6.0);
	CImg<double> result(transform_image<double>(img, t));
	cimg_library::CImgDisplay d1(img, "temp1");
	cimg_library::CImgDisplay d2(result, "temp2");
	result.save("result.png");

	while (!d1.is_closed() || !d2.is_closed()) {
		d1.wait();
		d2.wait();
	}
#else
	SqMatrix mat(3);
	mat(0,0) = 0.9;
	mat(0,1) = 0.258;
	mat(0,2) = -182;
	mat(1,0) = -0.153;
	mat(1,1) = 1.44;
	mat(1,2) = 58;
	mat(2,0) = -0.000306;
	mat(2,1) = 0.000731;
	mat(2,2) = 1;

	//mat = mat.transpose();

	Transformation t(mat);
	CImg<double> img("lincoln.png");
	CImg<double> result = transform_image<double>(img, t);
	img_display(result);

	return 0;
#endif
}

//-----------------------------------------------------------------------------------------

double euclidean_distance(const SiftDescriptor &input1, const SiftDescriptor &input2)
{
	double distance = 0;
		for (int i = 0; i < 128; i++)
		{
			distance=distance+(input1.descriptor[i] - input2.descriptor[i]) * (input1.descriptor[i] - input2.descriptor[i]);
		}
		distance=sqrt(distance);
		return distance;
}

//
// Generate a random number between 0 and 1
// return a uniform number in [0,1].
CImg<double> compute_X()
{
	CImg<double> rand_mat(5, 128);
	rand_mat.rand(0,1.0);
	return rand_mat;
}

bool nearest_neighbour(const vector< double> &input1, const vector< double> &input2)
{
	for (int i = 0; i < 10; i++)
	{
	if (input1[i] != input2[i])
	{
	return false;
	}
	}
	return true;
}

bool compare_summary(const CImg<double> &src, const CImg<double> &target,int a, int b)
{
	for (int i = 0; i < 5; i++)
	{
	//if (abs(src(i, a) - target(i, b)) >= tolerance)
	if (src(i, a) != target(i, b))
	{
	return false;
	}
	}
	return true;
}

double projection_function(const vector<SiftDescriptor> &input1, const vector<SiftDescriptor> &input2,const CImg<double> &X,vector< pair<int, int> > &pairs)
{
	double w = 255;
	int K = 5;
	int M = input1.size();
	int N = input2.size();
	CImg<double> src_summary_vecs(K, M);
	for (int y = 0; y < M; y++) {
		for (int j = 0; j < K; j++) {
			for (int i = 0; i < 128; i++) {
				src_summary_vecs(j, y) += X(j, i) * input1[i].descriptor[i] / w;
			}
			src_summary_vecs(j, y) = (int) src_summary_vecs(j, y);
		}
	}

	CImg<double> target_summary_vecs(K, N);
	for (int y = 0; y < N; y++) {
		for (int j = 0; j < K; j++) {
			for (int i = 0; i < 128; i++) {
				target_summary_vecs(j, y) += X(j, i) * input2[i].descriptor[i] / w;
			}
			target_summary_vecs(j, y) = (int) target_summary_vecs(j, y);
		}
	}

	double dist = 0.0;
	int min_index = -1, second_index;

	std::map<int, pair<int, double> > match_map;

	for (int i = 0; i < M; i++) {
		double min_dist = std::numeric_limits<double>::infinity(), second_dist;
		int j;
		for (j = 0; j < N; j++) {
			if (compare_summary(src_summary_vecs, target_summary_vecs, i, j)) {
				double diff = euclidean_distance(input1[i], input2[j]);
				printf("%d<>%d:%f\n", i, j, diff);
				if (min_dist > diff) {
					second_dist = min_dist;
					second_index = min_index;
					min_dist = diff;
					min_index = j;
				} else if (second_dist > diff) {
					second_dist = diff;
					second_index = j;
				}
			}
		}
		if (min_dist == std::numeric_limits<double>::infinity()) {
			for (j = 0; j < N; j++) {
				double diff = euclidean_distance(input1[i], input2[j]);
				if (min_dist > diff) {
					second_dist = min_dist;
					second_index = min_index;
					min_dist = diff;
					min_index = j;
				} else if (second_dist > diff) {
					second_dist = diff;
					second_index = j;
				}
			}
		}
		printf("dist: %f, dist2: %f\n", min_dist, second_dist);
		dist += min_dist;
		//if (min_dist < 260 && second_dist - min_dist > 100)
		if (second_dist - min_dist > 100) {
			map<int, pair<int, double> >::iterator pos = match_map.find(
					min_index);
			if (pos == match_map.end()) {
				match_map[min_index] = pair<int, double>(i, min_dist);
			} else if (pos->second.second > min_dist) {
				match_map[min_index] = pair<int, double>(i, min_dist);
			}

			//pairs.push_back(pair<int, int>(i, min_index));
		}

	}
	for (map<int, pair<int, double> >::iterator it = match_map.begin();
			it != match_map.end(); it++) {
		pairs.push_back(pair<int, int>(it->second.first, it->first));
	}

	for (int i = 0; i < pairs.size(); i++) {
		printf("%d -->> %d\n", pairs[i].first, pairs[i].second);
	}

	return dist;
}

void mark_descriptors(CImg<double> &input_image, const vector<SiftDescriptor> & descriptors, int color)
{
	for(int i=0; i<descriptors.size(); i++)
	{
	cout << "descriptor #" << i << ": x=" << descriptors[i].col << " y=" << descriptors[i].row << " descriptor=(";
	for(int l=0; l<128; l++)
	cout << descriptors[i].descriptor[l] << "," ;
	cout << ")" << endl;

	for(int j=0; j<5; j++)
	for(int k=0; k<5; k++)
	if(j==2 || k==2)
	{
	int x_d = descriptors[i].col+k;
	int y_d = descriptors[i].row+j;
	x_d = min(x_d, input_image.width() - 1);
	y_d = min(y_d, input_image.height() - 1);
	input_image(x_d, y_d, 0, 0)=0;
	input_image(x_d, y_d, 0, 1)=0;
	input_image(x_d, y_d, 0, 2)=0;
	}

	}
}

void copying(CImg<double> &target, const CImg<double> &src)
{
	for (int i = 0; i < src.height(); i++)
	{
	for (int j = 0; j < src.width(); j++)
	{
	target(j, i, 0, 0) = src(j, i, 0, 0);
	target(j, i, 0, 1) = src(j, i, 0, 1);
	target(j, i, 0, 2) = src(j, i, 0, 2);
	}
	}
}

CImg<double> stack_image(const CImg<double> &img1, const CImg<double> &img2)
{
	int cols = img1.width() + img2.width();
	int rows = max(img1.height(), img2.height());

	CImg<double> newimg(cols, rows, 1, 3);
	newimg = 0.0;

	copying(newimg, img2);
	newimg.shift(img1.width());
	copying(newimg, img1);

	return newimg;
}

CImg<double> compute_ransac(const std::vector<MappedCoordinates>& input) {
	int i, j, k, select_index;
	int inliners = 0;
	int max_inliners = 0;
	int max_inliners_index = 0;
	int limit = 4;

	double min_norm = std::numeric_limits<double>::infinity();

	CImg<double> A(8, 8, 1, 1);
	CImg<double> B(1, 8, 1, 1);

	CImg<double> transformation_matrix(3, 3, 1, 1);
	CImg<double> best_transformation_matrix(3, 3, 1, 1);
	CImg<double> new_coordinates(1, 3, 1, 1);
	CImg<double> old_coordinates(1, 3, 1, 1);

	double x_original;
	double y_original;
	double x_transformed;
	double y_transformed;
	int iterations = 0;
	cout << "IN RANSAC \n";
	while (iterations < 100) {
		static unsigned total_tries = 0;
		total_tries++;
		if (total_tries >= 3000000) {
			cout << "failed to find proper correspondances.\n";
			exit(1);
		}
		for (j = 0; j < 8; j++) {
			if (j % 2 == 0) {
				select_index = ((double) rand()) / RAND_MAX * input.size();
				MappedCoordinates mp = input[j];
				x_original = mp.x1;
				y_original = mp.y1;
				x_transformed = mp.x2;
				y_transformed = mp.y2;

				A(0, j) = x_original;
				A(1, j) = y_original;
				A(2, j) = 1;
				A(3, j) = 0;
				A(4, j) = 0;
				A(5, j) = 0;
				A(6, j) = -(x_original * x_transformed);
				A(7, j) = -(y_original * x_transformed);

				B(0, j) = x_transformed;
			} else {
				A(0, j) = 0;
				A(1, j) = 0;
				A(2, j) = 0;
				A(3, j) = x_original;
				A(4, j) = y_original;
				A(5, j) = 1;
				A(6, j) = -(x_original * y_transformed);
				A(7, j) = -(y_original * y_transformed);

				B(0, j) = y_transformed;
			}
		}
		CImg<double> X = B.solve(A);
		k = 0;
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				transformation_matrix(j, i) = X(0, k);
				k++;
			}
		}
		transformation_matrix(2, 2) = 1;

		old_coordinates(0, 2) = 1;
		inliners = 0;

		double normx;
		if (transformation_matrix.magnitude(2) > 500) {
			continue;
		}
		CImg<double> tmp = transformation_matrix;
		tmp.invert(1);
		if (tmp.magnitude(2) > 500) {
			continue;
		}
		normx = max(transformation_matrix.magnitude(2), tmp.magnitude(2));

		for (i = 0; i < input.size(); i++) {
			MappedCoordinates mp = input[j];
			old_coordinates(0, 0) = mp.x1;
			old_coordinates(0, 1) = mp.y1;

			new_coordinates = transformation_matrix * old_coordinates;

			if ((new_coordinates(0, 0) >= (mp.x2 - limit)
					&& new_coordinates(0, 0) <= (mp.x2 + limit))
					&& (new_coordinates(0, 1) >= (mp.y2 - limit)
							&& new_coordinates(0, 1) <= (mp.y2 + limit))) {
				inliners++;
			}
		}
		if (inliners < 4) {
			continue;
		} else {
			cout << "in layers: " << inliners << endl;
		}

		if (max_inliners < inliners) {
			max_inliners = inliners;
			max_inliners_index = select_index;
			cout << "norm = " << normx << endl;
			for (i = 0; i < 3; i++) {
				for (j = 0; j < 3; j++) {
					best_transformation_matrix(j, i) = transformation_matrix(j,
							i);

				}
			}
		}
		iterations++;

		total_tries = 0;
	}
	cout << "MAX=" << max_inliners_index << "\n";
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			cout << best_transformation_matrix(j, i) << " ";

		}
		cout << "\n";
	}

	cout << best_transformation_matrix.magnitude(1) << endl;
	return best_transformation_matrix;
}


//----------------------------------------------------------------------------------------


int main(int argc, char **argv)
{
	return temp();
	try {
		if(argc < 2)
		{
			cout << "Insufficent number of arguments; correct usage:" << endl;
			cout << "    a2-p1 part_id ..." << endl;
			return -1;
		}

		string part = argv[1];

		// Sift detector
		if(part == "part1.1")
		{
			if(argc < 4)
			{
				cout << "Insufficent number of arguments." << endl;
				return -1;
			}

			CImg<double> input1(argv[2]);
			CImg<double> input2(argv[3]);

			double matches = sift_matching(input1, input2, part);
			cout<<"The Number of Sift Matches: "<<matches<<endl;
		}
		else if(part == "part1.2")
		{
			if(argc < 4)
			{
				cout << "Insufficent number of arguments." << endl;
				return -1;
			}

			CImg<double> input(argv[2]);

			// creating a vector to store the sift matches for each of the image
			vector<Image> images;

			// take in all the images from the cammand line and calculate sift matches of the descriptors
			for(int i=3; i<argc; i++)
			{
				CImg<double> img(argv[i]);
				Image I;
				I.setParameters(argv[i], sift_matching(input, img, part));
				images.push_back(I);
			}

			// sort according to the sift match count
			std::sort(images.begin(), images.end());

			// printing the results
			cout<<"Top matches for Input Image: "<<argv[2]<<endl<<"Image Name: \t\t"<<"Count: "<<endl;
			for(int i = 0; i < images.size(); i++)
			{
				cout<<images[i].getName()<<"\t\t  "<<images[i].getCount()<<endl;
			}

		}

		else if (part == "part1.4") {

				string inputFile = argv[2];
				CImg<double> input_image(inputFile.c_str());

				CImg<double> gray = input_image.get_RGBtoHSI().get_channel(2);
				vector<SiftDescriptor> descriptors = Sift::compute_sift(gray);

				list< pair<double, int> > ordered; // ( (dist, index), ... )

				vector< CImg<double> > targets;
				vector< vector< SiftDescriptor > > desc_records;
				vector< vector< pair< int, int > > > all_pic_pairs;
				const int first_target_i = 2;
				for (int nfile = first_target_i; nfile < argc; nfile ++)
				{
				targets.push_back(CImg<double>(argv[nfile]));
				CImg<double> &target_image = targets.back();

				// convert image to grayscale
				CImg<double> gray = target_image.get_RGBtoHSI().get_channel(2);
				vector<SiftDescriptor> target_descriptors = Sift::compute_sift(gray);
				desc_records.push_back(target_descriptors);

				mark_descriptors(target_image, target_descriptors, 0);

				all_pic_pairs.push_back(vector< pair<int, int> >());
				vector< pair<int, int> > &pairs = all_pic_pairs.back();
				CImg<double> X = compute_X();
				double distance = projection_function(descriptors, target_descriptors, X, pairs);

				list< pair<double, int> >::iterator lsit;
				for (lsit = ordered.begin(); lsit != ordered.end(); lsit++)
				{
				if (lsit->first >= distance)
				{
				break;
				}
				}
				printf("argv[%d] : %f\n", nfile, distance);

				//target_image()

				ordered.insert(lsit, pair<double, int>(distance, nfile - first_target_i));
				}

				for (list<pair<double, int> >::iterator it = ordered.begin();
					it != ordered.end(); it++)
					{
					int ind = it->second;
					printf("%s\n", argv[ind + first_target_i]);

					CImg<double> &target_image = targets[ind];

					vector< pair<int, int> > &pairs = all_pic_pairs[ind];

					gray = target_image.get_RGBtoHSI().get_channel(2);
					//vector<SiftDescriptor> target_descriptors = Sift::compute_sift(gray);
					vector<SiftDescriptor> &target_descriptors = desc_records[ind];

					//double distance = sift_matching_with_random_vecotr(descriptors, target_descriptors, rand_mat, pairs);

					CImg<double> paired = stack_image(input_image, target_image);

					const unsigned char color[] = { 255,128,64 };
					for (int i = 0; i < pairs.size(); i++)
					{
					int x1 = descriptors[pairs[i].first].col;
					int y1 = descriptors[pairs[i].first].row;
					int x2 = target_descriptors[pairs[i].second].col;
					int y2 = target_descriptors[pairs[i].second].row;
					//printf("%d %d, %d %d\n", x1, y1, x2, y2);
					x2 += input_image.width();
					paired.draw_line(x1, y1, x2, y2, color);
					}

					CImgDisplay pair_disp(paired, "PAIRED");
					while (!pair_disp.is_closed())
					{
					pair_disp.wait();
					}

					paired.save("paired.png");
					}

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


