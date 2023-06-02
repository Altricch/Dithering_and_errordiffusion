#include <iostream>
#include <fstream>
#include <string>
#include <vector>


using namespace std;

vector< vector<float> > inputImage;
vector< vector<bool> > outputImage;


int height;
int width;

void read_image(char *filename){
    
    string type;
    int maxValue;
    int pixelValue;

    float accumulator;
    float counter;
    float average;

    accumulator = 0;
    counter = 0;
    average = 0;
    
    ifstream inputFile(filename);
    
    if(!inputFile.is_open()){
        cout<<"Unable to open InputImageFile"<<endl;
        return;
    }
    
    inputFile>>type>>width>>height>>maxValue;

    
    inputImage.resize(height);
    outputImage.resize(height);
    
    for (int i = 0; i < width; i++){
        inputImage[i].resize(width);
        outputImage[i].resize(width);
        for (int j = 0; j < height; j++){
            inputFile>>pixelValue;
            inputImage[i][j] = (float)pixelValue / maxValue;
            // cout << inputImage[i][j] << " Pixel value" << endl;

            accumulator += inputImage[i][j];
            counter += 1.0;
        }
    }
    average = accumulator / counter;
    cout << "Average intensity is:" << average << endl;
    
}


void write_image(char *filename){
    
    ofstream outputFile(filename);
    
    if(!outputFile.is_open()){
        cout<<"Unable to open OutputImageFile"<<endl;
        return;
    }
    
    outputFile<<"P1"<<endl<<width<<" "<<height<<endl<<endl;
    
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            outputFile<<!outputImage[i][j]<<" ";
        }
        outputFile<<endl;
    }
}

void convert_using_thresholding(void){
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) { 
            outputImage[i][j] = (float)(inputImage[i][j] > 0.5);
            // cout << outputImage[i][j]; Already normalized
        }
    }
}

void convert_using_dithering(void){


    // Test Case 1
    // float thresh_m[3][3];
    // thresh_m[0][0] = 2.0/9.0;
    // thresh_m[0][1] = 6.0/9.0;
    // thresh_m[0][2] = 4.0/9.0;
    // thresh_m[1][0] = 5.0/9.0;
    // thresh_m[1][1] = 0.0/9.0;
    // thresh_m[1][2] = 1.0/9.0;
    // thresh_m[2][0] = 8.0/9.0;
    // thresh_m[2][1] = 3.0/9.0;
    // thresh_m[2][2] = 7.0/9.0;


    // Test Case 2
    // float thresh_m[3][3];
    // thresh_m[0][0] = 7.0/9.0;
    // thresh_m[0][1] = 6.0/9.0;
    // thresh_m[0][2] = 4.0/9.0;
    // thresh_m[1][0] = 5.0/9.0;
    // thresh_m[1][1] = 6.0/9.0;
    // thresh_m[1][2] = 1.0/9.0;
    // thresh_m[2][0] = 8.0/9.0;
    // thresh_m[2][1] = 3.0/9.0;
    // thresh_m[2][2] = 7.0/9.0;

    // Test Case 3
    float thresh_m[4][4];
    thresh_m[0][0] = 2.0/16.0;
    thresh_m[0][1] = 16.0/16.0;
    thresh_m[0][2] = 3.0/16.0;
    thresh_m[0][3] = 16.0/16.0;
    thresh_m[1][0] = 10.0/16.0;
    thresh_m[1][1] = 6.0/16.0;
    thresh_m[1][2] = 11.0/16.0;
    thresh_m[1][3] = 7.0/16.0;
    thresh_m[2][0] = 4.0/16.0;
    thresh_m[2][1] = 14.0/16.0;
    thresh_m[2][2] = 1.0/16.0;
    thresh_m[2][3] = 15.0/16.0;
    thresh_m[3][0] = 12.0/16.0;
    thresh_m[3][1] = 8.0/16.0;
    thresh_m[3][2] = 9.0/16.0;
    thresh_m[3][3] = 5.0/16.0;

    int thres_size = sizeof thresh_m / sizeof thresh_m[0];

    for (int i = 0; i < thres_size; i++) {
        for (int j = 0; j < thres_size; j++){
            cout << thresh_m[i][j] << " ";
        }
        // Newline for new row
        cout << endl;
    }       

    float v;
    int i;
    int j;

    for (int x = 0; x < height; x++) {
        for (int y = 0; y < width; y++) { 
            v = inputImage[x][y];
            i = x % thres_size;
            j = y % thres_size;

            if (v >= thresh_m[i][j]) {
                outputImage[x][y] = 1;
            } else {
                outputImage[x][y] = 0;
            }
        }
    }
}

void convert_using_error_diffusion(void){
    
    // Initialize AccErr 
    float AccErr[width][height]; 
    for (int x = 0; x < height; x++) {
        for (int y = 0; y < width; y++) {
            AccErr[x][y] = 0;
        }
    }

    float v;
    float err;

    // Test 1 
    float threshold = 0.5;
    float err1 = 7.0/16.0;
    float err2 = 5.0/16.0;
    float err3 = 3.0/16.0;
    float err4 = 1.0/16.0;

     // Test 2
    // float threshold = 0.36;
    // float err1 = 7.0/16.0;
    // float err2 = 5.0/16.0;
    // float err3 = 3.0/16.0;
    // float err4 = 1.0/16.0;

    // Test 3
    // float threshold = 0.5;
    // float err1 = 1.0/16.0;
    // float err2 = 3.0/16.0;
    // float err3 = 5.0/16.0;
    // float err4 = 7.0/16.0;

    for (int x = 0; x < height; x++) {
        for (int y = 0; y < width; y++) {
            v = inputImage[x][y] + AccErr[x][y];
            if (v >= threshold) {
                outputImage[x][y] = 1;
                err = v - 1; 
            } else {
                outputImage[x][y] = 0;
                err = v;
            }
            AccErr[x % height][(y+1) % width] += err1 * err;
            AccErr[(x+1) % height][(y-1) % width] += err2 * err;
            AccErr[(x+1) % height][y % width] += err3 * err;
            AccErr[(x+1) % height][(y+1) % width] += err4 * err;
        }
    }
}

// BASED ON STUCKI
void convert_using_error_diffusion_extend(void){
    
    // Initialize AccErr 
    float AccErr[width][height]; 
    for (int x = 0; x < height; x++) {
        for (int y = 0; y < width; y++) {
            AccErr[x][y] = 0;
        }
    }

    float v;
    float err;

    float threshold = 0.5;
    float err1 = 8.0/42.0;
    float err2 = 4.0/42.0;
    float err3 = 2.0/42.0;
    float err4 = 4.0/42.0;
    float err5 = 8.0/42.0;
    float err6 = 4.0/42.0;
    float err7 = 2.0/42.0;
    float err8 = 1.0/42.0;
    float err9 = 2.0/42.0;
    float err10 = 4.0/42.0;
    float err11 = 2.0/42.0;
    float err12 = 1.0/42.0;
    

    for (int x = 0; x < height; x++) {
        for (int y = 0; y < width; y++) {
            v = inputImage[x][y] + AccErr[x][y];
            if (v >= threshold) {
                outputImage[x][y] = 1;
                err = v - 1; 
            } else {
                outputImage[x][y] = 0;
                err = v;
            }
            AccErr[x % height][(y+1) % width] += err1 * err;
            AccErr[x % height][(y+2) % width] += err2 * err;

            AccErr[(x+1) % height][(y-2) % width] += err3 * err;
            AccErr[(x+1) % height][(y-1) % width] += err4 * err;
            AccErr[(x+1) % height][(y) % width] += err5 * err;
            AccErr[(x+1) % height][(y+1) % width] += err6 * err;
            AccErr[(x+1) % height][(y+2) % width] += err7 * err;

            AccErr[(x+2) % height][(y-2) % width] += err8 * err;
            AccErr[(x+2) % height][(y-1) % width] += err9 * err;
            AccErr[(x+2) % height][(y) % width] += err10 * err;
            AccErr[(x+2) % height][(y+1) % width] += err11 * err;
            AccErr[(x+2) % height][(y+2) % width] += err12 * err;
        }
    }
}



int main(int argc, char **argv) {

    if(argc < 4){
        cout<<"Usage: Halftoning IntputImageFileName OutputImageFileName Method"<<endl;
        cout<<"Method = Thresholding | Dithering | ErrorDiffusion"<<endl;
        return 0;
    }
    
    read_image(argv[1]);
    
    if (strcmp(argv[3],"Thresholding") == 0) convert_using_thresholding();
    else{
        if (strcmp(argv[3],"Dithering") == 0) convert_using_dithering();
        else{
            if (strcmp(argv[3],"ErrorDiffusion") == 0) convert_using_error_diffusion();
            else{
                if (strcmp(argv[3],"ErrorDiffusionExtension") == 0) convert_using_error_diffusion_extend();
                    else{
                        cout<<"Usage: Halftoning IntputImageFileName OutputImageFileName Method"<<endl;
                        cout<<"Method = Thresholding | Dithering | ErrorDiffusion"<<endl;
                        return 0;
                    }
            }

        }
    }
    
    write_image(argv[2]);
    
    return 0;
}
