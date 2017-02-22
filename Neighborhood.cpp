#include "SPH.h"
#include <cmath>


using namespace std;


/// Naive method to find the neighbors of all particles.
// All-pair search algorithm
/*
Inputs:
- pos: array with the particle positions (x,y,z components)
- kh: support size of the kernel
- values: nonzero values of the incidence matrix (r_ab for the moment...)
- row: row indices of the nonzero values
- column: column indices of the nonzero values
Output:
/
*/
void neighborAllPair (std::vector<double> &pos,
                         double kh,
                         std::vector<double> &values,
                         std::vector<int> &row,
                         std::vector<int> &column)
{
    double kh2 = pow(kh,2);

    // For each particle, browse all other particles and compute the distance
    for(int i=0; i<pos.size(); i=i+3)
    {
        for(int j=i; j<pos.size(); j=j+3)
        {
            double r2 = distance(pos, i/3, j/3);
            if( r2 < kh2 )
            {
                //std::cout << r2 << "\t: ";
                //std::cout << i << " " << j << "\n";
                values.push_back(r2); // The distance bewteen the two found neighbors
                row.push_back(i); // The one we search the neighbors of
                column.push_back(j); // The neighbor we have just found
            }
        }
    }

}



/// Advanced method to find the neighbors of all particles.

// Linked-list algorithm
/*
Inputs:
- pos: array with the particle positions (x,y,z components)
- l: lowest (x,y,z) point in the numerical domain
- u: highest (x,y,z) point in the numerical domain
- kh: support size of the kernel
- values: nonzero values of the incidence matrix (r_ab for the moment...)
- row: row indices of the nonzero values
- column: column indices of the nonzero values
Output:
/
*/
void neighborLinkedList (std::vector<double> &pos,
                         double l[3],
                         double u[3],
                         double kh,
                         std::vector<double> &values,
                         std::vector<int> &row,
                         std::vector<int> &column)
{
    double kh2 = pow(kh,2);

    // Box definition
    vector<vector<int> > boxes;
    int nBoxesX = ceil((u[0] - l[0])/kh); // Extra box if non integer quotient
    int nBoxesY = ceil((u[1] - l[1])/kh);
    int nBoxesZ = ceil((u[2] - l[2])/kh);

    int nBoxes = nBoxesX * nBoxesY * nBoxesZ;

    for(int i=0 ; i<nBoxes ; i++)
    {
        vector<int> boxContent;
        boxes.push_back(boxContent);
    }
    //std::cout << nBoxesX << " " << nBoxesY << " " << nBoxesZ << " \n";

    // Sort the particles
    int nPart = pos.size()/3;
    // Box identifier variables
    int boxX;
    int boxY;
    int boxZ;
    double temp;
    for(int i=0 ; i<nPart ; i++)
    {
        temp = (pos[3*i] - l[0])/kh; // Integer division
        boxX = (temp < nBoxesX-1) ? temp : nBoxesX-1;
        temp = (pos[3*i+1] - l[1])/kh;
        boxY = (temp < nBoxesY-1) ? temp : nBoxesY-1;
        temp = (pos[3*i+2] - l[2])/kh;
        boxZ = (temp < nBoxesZ-1) ? temp : nBoxesZ-1;
        // Put the particle identifier in the corresponding box array
        boxes[boxX*nBoxesY*nBoxesZ + boxY*nBoxesZ + boxZ].push_back(i);
        //std::cout << boxX*nBoxesY*nBoxesZ + boxY*nBoxesZ + boxZ << " \n";
    }

    // Search for their neighbors
    int particleID;
    // Spans the boxes
    for(int box=0 ; box<nBoxes ; box++)
    {
        // Determines the list of surronding boxes (boundaries -> not trivial)
        std::vector<int> surrBoxes;
        surroundingBoxes(box, nBoxesX, nBoxesY, nBoxesZ, surrBoxes);
        // Spans the particles in the box
        for(int part=0 ; part<boxes[box].size() ; part++)
        {
            particleID = boxes[box][part];
            // Spans the surrounding boxes
            for(int surrBox = 0 ; surrBox < surrBoxes.size() ; surrBox++)
            {
                // Spans the higher index particles in the box (symmetry)
                for(int i=0 ; i<boxes[surrBox].size() ; i++)
                {
                    int potNeighborID = boxes[surrBox][i];
                    if(potNeighborID >= particleID)
                    {
                        double r2 = distance(pos, particleID, potNeighborID);
                        if(r2<kh2)
                        {
                            //std::cout << r2 << "\t: ";
                            //std::cout << particleID << " " << potNeighborID << "\n";
                            values.push_back(r2); // The distance bewteen the two found neighbors
                            row.push_back(particleID); // The one we search the neighbors of
                            column.push_back(potNeighborID); // The neighbor we have just found
                        }
                    }
                }
            }
        }
    }

}

// Gives the list of the surrounding boxes
void surroundingBoxes(int box, int nBoxesX, int nBoxesY, int nBoxesZ, std::vector<int> &surrBoxes)
{
    surrBoxes.push_back(box);
    int nx=nBoxesX;
    int ny=nBoxesY;
    int nz=nBoxesZ;
    int index_x;
    int index_y;
    int index_z;
    index_z=box/(nBoxesX*nBoxesY);
    index_y=(box-index_z*nBoxesX*nBoxesY)/nBoxesX;
    index_x=box-index_z*nBoxesX*nBoxesY-index_y*nBoxesX;
    if(index_x>0 &&  index_y>0  && index_z>0 && index_x<(nBoxesX-1) &&  index_y<(nBoxesY-1)  && index_z<(nBoxesZ-1))
    {
      // direct face neigbour
      surrBoxes.push_back(box+1);// right
      surrBoxes.push_back(box-1);// left
      surrBoxes.push_back(box+nx);// back
      surrBoxes.push_back(box-nx);// front
      surrBoxes.push_back(box+nx*ny);// up
      surrBoxes.push_back(box-nx*ny);// down
      // corner + arete neigbour
      // right up
      surrBoxes.push_back(box+1 +nx*ny);
      surrBoxes.push_back(box+1 +nx*ny+nx);
      surrBoxes.push_back(box+1 +nx*ny-nx);
      // right down
      surrBoxes.push_back(box+1 -nx*ny);
      surrBoxes.push_back(box+1 -nx*ny+nx);
      surrBoxes.push_back(box+1 -nx*ny-nx);
      // left up
      surrBoxes.push_back(box-1 +nx*ny);
      surrBoxes.push_back(box-1 +nx*ny+nx);
      surrBoxes.push_back(box-1 +nx*ny-nx);
      // left down
      surrBoxes.push_back(box-1 -nx*ny);
      surrBoxes.push_back(box-1 -nx*ny+nx);
      surrBoxes.push_back(box-1 -nx*ny-nx);
      //  front-back up
      surrBoxes.push_back(box+nx +nx*ny);
      surrBoxes.push_back(box-nx +nx*ny);
      //  front-back down
      surrBoxes.push_back(box+nx -nx*ny);
      surrBoxes.push_back(box-nx -nx*ny);
      // front left right
      surrBoxes.push_back(box-nx +1);
      surrBoxes.push_back(box-nx -1);
      // back left right
      surrBoxes.push_back(box+nx +1);
      surrBoxes.push_back(box+nx -1);
    }
    if(index_x==0)// no left block to consider , for all index_y and index_z
    {
        if(index_y==0)// no front block to consider , for all index_z
        {
            if(index_z==0)// no down block to consider
            {
              surrBoxes.push_back(box+1);// right
              surrBoxes.push_back(box+nx);// back
              surrBoxes.push_back(box+nx*ny);// up
              // right up
              surrBoxes.push_back(box+1 +nx*ny);
              surrBoxes.push_back(box+1 +nx*ny+nx);
              surrBoxes.push_back(box+1 +nx*ny-nx);
              //back up
              surrBoxes.push_back(box+nx +nx*ny);
              // back right
              surrBoxes.push_back(box+nx +1);
            }
            else if(index_z==(nz-1))// no up block to consider
            {
                surrBoxes.push_back(box+1);// right
                surrBoxes.push_back(box+nx);// back
                surrBoxes.push_back(box-nx*ny);// down
                // right down
                surrBoxes.push_back(box+1 -nx*ny);
                surrBoxes.push_back(box+1 -nx*ny+nx);
                surrBoxes.push_back(box+1 -nx*ny-nx);
                // back down
                surrBoxes.push_back(box+nx -nx*ny);
                // back right
                surrBoxes.push_back(box+nx +1);
                // back up
                surrBoxes.push_back(box+nx +nx*ny);
            }
            else // can take up and down
            {
                surrBoxes.push_back(box+1);// right
                surrBoxes.push_back(box+nx);// back
                surrBoxes.push_back(box+nx*ny);// up
                surrBoxes.push_back(box-nx*ny);// down
                // right up
                surrBoxes.push_back(box+1 +nx*ny);
                surrBoxes.push_back(box+1 +nx*ny+nx);
                surrBoxes.push_back(box+1 +nx*ny-nx);
                // right down
                surrBoxes.push_back(box+1 -nx*ny);
                surrBoxes.push_back(box+1 -nx*ny+nx);
                surrBoxes.push_back(box+1 -nx*ny-nx);
                //back down
                surrBoxes.push_back(box+nx -nx*ny);
                // back right
                surrBoxes.push_back(box+nx +1);
                // back up
                surrBoxes.push_back(box+nx +nx*ny);
            }
        }
        else if (index_y==(ny-1))// no back block
        {
              if(index_z==0)// no down block to consider
              {
                surrBoxes.push_back(box+1);// right
                surrBoxes.push_back(box-nx);// front
                surrBoxes.push_back(box+nx*ny);// up
                // right up
                surrBoxes.push_back(box+1 +nx*ny);
                surrBoxes.push_back(box+1 +nx*ny+nx);
                surrBoxes.push_back(box+1 +nx*ny-nx);
                // front up
                surrBoxes.push_back(box-nx +nx*ny);
                // front  right
                surrBoxes.push_back(box-nx +1);
              }
              else if(index_z==(nz-1))// no up block to consider
              {
                  surrBoxes.push_back(box+1);// right
                  surrBoxes.push_back(box-nx);// front
                  surrBoxes.push_back(box-nx*ny);// down
                  // right down
                  surrBoxes.push_back(box+1 -nx*ny);
                  surrBoxes.push_back(box+1 -nx*ny+nx);
                  surrBoxes.push_back(box+1 -nx*ny-nx);
                  // front down
                  surrBoxes.push_back(box-nx -nx*ny);
                  // front  right
                  surrBoxes.push_back(box-nx +1);
              }
              else // can take up and down
              {
                surrBoxes.push_back(box+1);// right
                surrBoxes.push_back(box-nx);// front
                surrBoxes.push_back(box+nx*ny);// up
                surrBoxes.push_back(box-nx*ny);// down
                // right up
                surrBoxes.push_back(box+1 +nx*ny);
                surrBoxes.push_back(box+1 +nx*ny+nx);
                surrBoxes.push_back(box+1 +nx*ny-nx);
                // right down
                surrBoxes.push_back(box+1 -nx*ny);
                surrBoxes.push_back(box+1 -nx*ny+nx);
                surrBoxes.push_back(box+1 -nx*ny-nx);
                // front up
                surrBoxes.push_back(box-nx +nx*ny);
                // fornt down
                surrBoxes.push_back(box-nx -nx*ny);
                // front  right
                surrBoxes.push_back(box-nx +1);
              }
        }
        else// can take front and back
        {
              if(index_z==0)// no down block to consider
              {
                surrBoxes.push_back(box+1);// right
                surrBoxes.push_back(box+nx);// back
                surrBoxes.push_back(box-nx);// front
                surrBoxes.push_back(box+nx*ny);// up
                // right up
                surrBoxes.push_back(box+1 +nx*ny);
                surrBoxes.push_back(box+1 +nx*ny+nx);
                surrBoxes.push_back(box+1 +nx*ny-nx);
                //  front-back up
                surrBoxes.push_back(box+nx +nx*ny);
                surrBoxes.push_back(box-nx +nx*ny);
                // front  right
                surrBoxes.push_back(box-nx +1);
                // back  right
                surrBoxes.push_back(box+nx +1);
              }
              else if(index_z==(nz-1))// no up block to consider
              {
                surrBoxes.push_back(box+1);// right
                surrBoxes.push_back(box+nx);// back
                surrBoxes.push_back(box-nx);// front
                surrBoxes.push_back(box-nx*ny);// down
                // right down
                surrBoxes.push_back(box+1 -nx*ny);
                surrBoxes.push_back(box+1 -nx*ny+nx);
                surrBoxes.push_back(box+1 -nx*ny-nx);
                //  front-back down
                surrBoxes.push_back(box+nx -nx*ny);
                surrBoxes.push_back(box-nx -nx*ny);
                // front  right
                surrBoxes.push_back(box-nx +1);
                // back right
                surrBoxes.push_back(box+nx +1);
              }
              else // can take up and down
              {
                surrBoxes.push_back(box+1);// right
                surrBoxes.push_back(box+nx);// back
                surrBoxes.push_back(box-nx);// front
                surrBoxes.push_back(box+nx*ny);// up
                surrBoxes.push_back(box-nx*ny);// down
                // right up
                surrBoxes.push_back(box+1 +nx*ny);
                surrBoxes.push_back(box+1 +nx*ny+nx);
                surrBoxes.push_back(box+1 +nx*ny-nx);
                // right down
                surrBoxes.push_back(box+1 -nx*ny);
                surrBoxes.push_back(box+1 -nx*ny+nx);
                surrBoxes.push_back(box+1 -nx*ny-nx);
                //  front-back up
                surrBoxes.push_back(box+nx +nx*ny);
                surrBoxes.push_back(box-nx +nx*ny);
                //  front-back down
                surrBoxes.push_back(box+nx -nx*ny);
                surrBoxes.push_back(box-nx -nx*ny);
                // front  right
                surrBoxes.push_back(box-nx +1);
                // back  right
                surrBoxes.push_back(box+nx +1);
              }
        }
    }
    else if (index_x==(nx-1)) // no right block
    {
        if(index_y==0)// no front block to consider , for all index_z
        {
            if(index_z==0)// no down block to consider
            {
              
            }
            else if(index_z==(nz-1))// no up block to consider
            {

            }
            else // can take up and down
            {

            }
        }
        else if (index_y==(ny-1))// no back
        {
              if(index_z==0)// no down block to consider
              {

              }
              else if(index_z==(nz-1))// no up block to consider
              {

              }
              else // can take up and down
              {

              }
        }
        else// can take front and back
        {
              if(index_z==0)// no down block to consider
              {

              }
              else if(index_z==(nz-1))// no up block to consider
              {

              }
              else // can take up and down
              {

              }
        }
    }
    else // can take right and left block
    {
    if(index_y==0)// no front block to consider , for all index_z
    {
        if(index_z==0)// no down block to consider
        {

        }
        else if(index_z==(nz-1))// no up block to consider
        {

        }
        else // can take up and down
        {

        }
    }
    else if (index_y==(ny-1))// no back
    {
          if(index_z==0)// no down block to consider
          {

          }
          else if(index_z==(nz-1))// no up block to consider
          {

          }
          else // can take up and down
          {

          }
    }
    else// can take front and back
    {
          if(index_z==0)// no down block to consider
          {

          }
          else if(index_z==(nz-1))// no up block to consider
          {

          }// no else since already done
    }
    }
    return;
    }

// Gives the distance between two particles
double distance(std::vector<double> pos, int partA, int partB)
{
    return pow(pos[partA*3]-pos[partB*3],2)
               + pow(pos[partA*3+1]-pos[partB*3+1],2)
               + pow(pos[partA*3+2]-pos[partB*3+2],2);
}


// tree search algorithm
void neighborTree (double p[3], double h, std::vector<double> &pos)
{

}
