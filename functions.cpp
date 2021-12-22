#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include "functions.h"

using namespace std;

// TODO Write this function
int energy(const Pixel *const*image, int col, int row, int width, int height)
{
  int energy_x;
  int energy_y;

  int x_r = col + 1;
  int x_l = col -1; 
 
  //energy for x
  if(col == 0){
    x_l = width - 1;
  }
  if(col == (width - 1)){
    x_r = 0;   
  } 
  int r_red = image[x_r][row].r;
  int r_blue = image[x_r][row].b;
  int r_green = image[x_r][row].g;

  int l_red = image[x_l][row].r;
  int l_blue = image[x_l][row].b;
  int l_green = image[x_l][row].g;

  int red_energy_x = r_red - l_red;
  int blue_energy_x = r_blue - l_blue;
  int green_energy_x = r_green - l_green;

  energy_x = pow(red_energy_x,2) + pow(blue_energy_x,2) + pow(green_energy_x,2);

  // energy for y
  int y_u = row - 1;
  int y_d = row + 1;

  if(row == 0){
    y_u = height -1;
  }
  if(row == (height - 1)){
    y_d = 0;
  } 

  int u_red = image[col][y_u].r;
  int u_blue = image[col][y_u].b;
  int u_green = image[col][y_u].g;

  int d_red = image[col][y_d].r;
  int d_blue = image[col][y_d].b;
  int d_green = image[col][y_d].g;

  int red_energy_y = d_red - u_red;
  int blue_energy_y = d_blue - u_blue;
  int green_energy_y = d_green - u_green;

  energy_y = pow(red_energy_y,2) + pow(blue_energy_y,2) + pow(green_energy_y,2);
  
  //total energy
  int energy_tot = energy_x + energy_y;

  return energy_tot;
}

// TODO Write this function
int getVerticalSeam(const Pixel *const*image, int start_col, int width, int height, int* seam)
{
  seam[0] = start_col;

  int answer = 0;

  int col_m = start_col;

  int box_l = 0;
  int box_m = 0;
  int box_r = 0;

  for(int row = 0; row < height-1; row++){
    int col_l = col_m - 1;
    int col_r = col_m + 1;

    if(width == 1){
      for(int x = 0; x < height; x++){
        box_m = energy(image, col_m, x, width, height);
        answer += box_m;
      }
      return answer;
    }
    if(col_m == 0){
      box_m = energy(image, col_m, row + 1, width, height);
      box_r = energy(image, col_r , row + 1, width, height);

      if(box_r < box_m){
        col_m = col_r;
        seam[row + 1] = col_m;
      }
      else if(box_m <= box_r){
        col_m = col_m;
        seam[row+1] = col_m;
      }
    }

    
    else if(col_m == width - 1){
      int box_l = energy(image, col_l , row + 1, width, height);
      int box_m = energy(image, col_m, row + 1, width, height);

      if(box_l < box_m){
        col_m = col_l;
        seam[row + 1] = col_m;
      }

      else if(box_m <= box_l){
        col_m = col_m;
        seam[row+1] = col_m;
      }
    }
    else{
      int box_l = energy(image, col_l , row + 1, width, height);
      int box_m = energy(image, col_m, row + 1, width, height);
      int box_r = energy(image, col_r , row + 1, width, height);

      if(box_m <= box_l && box_m <= box_r){
        col_m = col_m;
        seam[row + 1] = col_m;
      }
      else if(box_r < box_m && box_r <= box_l){
        col_m = col_r;
        seam[row + 1] = col_m;
      }
      else{
        col_m = col_l;
        seam[row + 1] = col_m;
        }
      }   
  }
  for(int i = 0; i < height; i++){
    answer += energy(image , seam[i], i , width, height);
  }
  return answer;

  box_m +=1;
  box_r +=1;
  box_l +=1;
}

// TODO Write this function
void removeVerticalSeam(Pixel **image, int& width, int height, int *verticalSeam){
  for(int i = 0; i < height; i++){
    for(int temp = verticalSeam[i]; temp < width-1; temp++){
      image[temp][i] = image[temp+1][i];
    }
  }
  width--;
}

// TODO Write this function for extra credit
/*int getHorizontalSeam(const Pixel *const*image, int start_row, int width, int height, int* seam)
{
  return 0;
}

// TODO Write this function for extra credit
void removeHorizontalSeam(Pixel **image, int width, int& height, int *horizontalSeam)
{
  
}*/

int *findMinVerticalSeam(const Pixel *const*image, int width, int height)
{
  // initialize minSeam and minDistance to seam starting at first col (index 0)
  int *minSeam = new int[height]{0};
  int minDist = getVerticalSeam(image, 0, width, height, minSeam);

  int *candidateSeam = new int[height]{0};
  int candidateDistance = -1; // invalid distance

  // start at second col (index 1) since we initialized with first col (index 0)
  for (int col = 1; col < width; ++col)
  {
    candidateDistance = getVerticalSeam(image, col, width, height, candidateSeam);

    if (candidateDistance < minDist)
    { // new min
      //  swap min & candidate
      minDist = candidateDistance;
      int* temp = candidateSeam;
      candidateSeam = minSeam;
      minSeam = temp;
    }
  }

  // clean up 
  delete [] candidateSeam;

  return minSeam;
}

/*int *findMinHorizontalSeam(const Pixel *const*image, int width, int height)
{
  // initialize minSeam and minDistance to seam starting at first row (index 0)
  int *minSeam = new int[width]{0};
  int minDistance = getHorizontalSeam(image, 0, width, height, minSeam);

  int *candidateSeam = new int[width]{0};
  int candidateDistance = -1; // invalid distance

  // start at second row (index 1) since we initialized with first row (index 0)
  for (int row = 1; row < height; ++row)
  {
    candidateDistance = getHorizontalSeam(image, row, width, height, candidateSeam);

    if (candidateDistance < minDistance)
    { // new minimum
      //  swap min and candidate seams
      minDistance = candidateDistance;
      int* temp = candidateSeam;
      candidateSeam = minSeam;
      minSeam = temp;
    }
  }

    // clean up 
  delete [] candidateSeam;

  return minSeam;
}
*/

Pixel **createImage(int width, int height)
{
  cout << "Start createImage... " << endl;

  // Create a one dimensional array on the heap of pointers to Pixels
  //    that has width elements (i.e. the number of columns)
  Pixel **image = new Pixel *[width] {}; // initializes to nullptr

  for (int col = 0; col < width; ++col)
  { // loop through each column
    // assign that column to a one dimensional array on the heap of Pixels
    //  that has height elements (i.e. the number of rows)
    try
    {
      image[col] = new Pixel[height];
    }
    catch (std::bad_alloc &e)
    {
      // clean up already allocated arrays
      for (int i = 0; i < col; ++i)
      {
        delete[] image[i];
      }
      delete[] image;
      // rethrow
      throw e;
    }
  }

  // initialize cells
  // cout << "Initializing cells..." << endl;
  for (int row = 0; row < height; ++row)
  {
    for (int col = 0; col < width; ++col)
    {
      // cout << "(" << col << ", " << row << ")" << endl;
      image[col][row] = {0, 0, 0};
    }
  }
  cout << "End createImage... " << endl;
  return image;
}

void deleteImage(Pixel **image, int width)
{
  cout << "Start deleteImage..." << endl;
  // avoid memory leak by deleting the array
  for (int i = 0; i < width; ++i)
  {
    delete[] image[i];
  }
  delete[] image;
  image = nullptr;
  cout << "End deleteImage..." << endl;
}

bool isValidColor(int colorVal)
{
  if (colorVal < 0 || colorVal > 255)
  {
    return false;
  }
  return true;
}

Pixel ** loadImage(string filename, int &width, int &height)
{
  cout << "Start loadImage..." << endl;
  // remove
  ifstream ifs(filename);
  if (!ifs.is_open())
  {
    throw std::invalid_argument("Failed to open input file (" + filename + ")");
  }

  string type;
  ifs >> type; // should be P3
  if (toupper(type.at(0)) != 'P' || type.at(1) != '3')
  {
    throw std::domain_error("Not PPM type P3 (" + type + ")");
  }
  ifs >> width;
  // cout << "w and h: " << w << " " << h << endl;
  if (ifs.fail())
  {
    throw std::domain_error("Read non-integer value for width");
  }
  if (width <= 0)
  {
    ostringstream oss;
    oss << "Width in file must be greater than 0 (" << width << ")";
    throw std::domain_error(oss.str());
  }

  ifs >> height;
  if (ifs.fail())
  {
    cout << "Read non-integer value for height" << endl;
  }
  if (height <= 0)
  {
    ostringstream oss;
    oss << "Height in file must be greater than 0 (" << height << ")";
    throw std::domain_error(oss.str());
  }

  int colorMax = 0;
  ifs >> colorMax;
  if (ifs.fail())
  {
    throw std::domain_error("Read non-integer value for max color value");
  }
  if (colorMax != 255)
  {
    ostringstream oss;
    oss << "Max color value must be 255 (" << colorMax << ")";
    throw std::domain_error(oss.str());
  }

  // load image throws exceptions but we will let them pass through
  Pixel **image = createImage(width, height);

  for (int row = 0; row < height; ++row)
  {
    for (int col = 0; col < width; ++col)
    {
      // cout << "Pixel(" << col << ", " << row << ")" << endl;
      ifs >> image[col][row].r;
      if (ifs.fail() && !ifs.eof())
      { // bad input that is not caused by being at the end of the file
        throw std::domain_error("Read non-integer value for red");
      }
      if (!isValidColor(image[col][row].r))
      {
        ostringstream oss;
        oss << "Invalid color value for red (" << image[col][row].r << ")";
        throw std::domain_error(oss.str());
      }

      ifs >> image[col][row].g;
      if (ifs.fail() && !ifs.eof())
      { // bad input that is not caused by being at the end of the file
        throw std::domain_error("Read non-integer value for green");
      }
      if (!isValidColor(image[col][row].r))
      {
        ostringstream oss;
        oss << "Invalid color value for green (" << image[col][row].r + ")";
        throw std::domain_error(oss.str());
      }

      ifs >> image[col][row].b;
      if (ifs.fail() && !ifs.eof())
      { // bad input that is not caused by being at the end of the file
        throw std::domain_error("Read non-integer value for blue");
      }
      if (!isValidColor(image[col][row].r))
      {
        ostringstream oss;
        oss << "Invalid color value for blue (" << image[col][row].r + ")";
        throw std::domain_error(oss.str());
      }
    }
  }
  cout << "End loadImage..." << endl;
  return image;
}

void outputImage(string filename, const Pixel *const *image, int width, int height)
{
  cout << "Start outputImage..." << endl;
  // remove code
  // declare/define and open output file stream with filename
  ofstream ofs(filename);
  // ensure file is open
  if (!ofs.is_open())
  {
    throw std::invalid_argument("Error: failed to open output file - " + filename);
  }
  ofs << "P3" << endl;
  ofs << width << " " << height << endl;
  ofs << 255 << endl;
  for (int row = 0; row < height; ++row)
  {
    for (int col = 0; col < width; ++col)
    {
      ofs << image[col][row].r << " ";
      ofs << image[col][row].g << " ";
      ofs << image[col][row].b << " ";
    }
    ofs << endl;
  }
  cout << "End outputImage..." << endl;
}