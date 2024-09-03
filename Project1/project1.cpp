#include <iostream>
using namespace std;

class SparseRow {
    protected:
        int row; //Row#
        int col; //Column#
        int value; //We will assume that all our values will be integers
    public:
        SparseRow (); //default constructor; row=-1;col=-1;value=0
        ~SparseRow (); //destructor
        void display(); // print Row#, Column#, value
        friend ostream &operator<<(ostream &s, SparseRow sr);
        //other methods that are necessary such as get and set
        
        // Getter methods
        int getRow() const;
        int getCol() const;
        int getValue() const;

        // Setter methods
        void setRow(int r);
        void setCol(int c);
        void setValue(int v);
};

SparseRow::SparseRow()
{
    row = -1;
    col = -1;
    value = 0;
}

SparseRow::~SparseRow() {
    // No dynamically allocated resources to free
}

int SparseRow::getRow() const {
    return row;
}

int SparseRow::getCol() const {
    return col;
}

int SparseRow::getValue() const {
    return value;
}

// Setter method definitions
void SparseRow::setRow(int r) {
    row = r;
}

void SparseRow::setCol(int c) {
    col = c;
}

void SparseRow::setValue(int v) {
    value = v;
}

void SparseRow::display()
{
    cout << row << ", " << col << ", " << value << endl;
}

ostream &operator<<(ostream &s, SparseRow sr)
{
    sr.display();
    return s;
}

class SparseMatrix {
    protected:
        int numRows; //Number of rows of the original matrix
        int numCols; //Number of columns of the original matrix
        int commonValue; //read from input
        int numNonSparseValues;
        SparseRow* myMatrix; //Array
    public:
        SparseMatrix ();
        SparseMatrix (int n, int m, int cv, int numNSV); //Constructor
        SparseMatrix (SparseMatrix& M); //Copy Constructor
        ~SparseMatrix();

        SparseMatrix* Transpose (); //Matrix Transpose
        SparseMatrix* Multiply (SparseMatrix& M);
        SparseMatrix* Add (SparseMatrix& M);
        friend ostream& operator<< (ostream& s, SparseMatrix& sm);
        void displaySparseMatrix (); //Display the matrix in its sparse format
        void displayMatrix (); //Display the matrix in its matrix format
        //other methods that are necessary such as get and set
        void setMyMatrix(SparseRow* newMyMatrix[]);
};

SparseMatrix::SparseMatrix()
{
    numRows = 0;
    numCols = 0;
    commonValue = 0;
    numNonSparseValues = 0;
    myMatrix = NULL;
}

SparseMatrix::SparseMatrix(int rows, int cols, int cv, int numNSV)
{
    numRows = rows;
    numCols = cols;
    commonValue = cv;
    numNonSparseValues = numNSV;
    myMatrix = new SparseRow[numNonSparseValues];
    int i = 0;
    for(int j = 0; j < numRows; j++){
        for(int k = 0; k < numCols; k++){
            int val;
            cin >> val;
            if(val != commonValue){
                myMatrix[i].setRow(j);
                myMatrix[i].setCol(k);
                myMatrix[i].setValue(val);
                i++;
            }
        }
    }
    
}

SparseMatrix::SparseMatrix(SparseMatrix& M)
{
    numRows = M.numRows;
    numCols = M.numCols;
    commonValue = M.commonValue;
    numNonSparseValues = M.numNonSparseValues;
    myMatrix = new SparseRow[numNonSparseValues];
    for(int i = 0; i < numNonSparseValues; i++){
        myMatrix[i] = M.myMatrix[i];
    }
}


SparseMatrix::~SparseMatrix()
{
    delete[] myMatrix;
}

void SparseMatrix::setMyMatrix(SparseRow* newMyMatrix[]) {
    myMatrix = *newMyMatrix;
}

void SparseMatrix::displaySparseMatrix()
{
   for(int i = 0; i < numNonSparseValues; i++){
       myMatrix[i].display();
   }
}

void SparseMatrix:: displayMatrix(){
    int i = 0;
    for(int j = 0; j < numRows; j++){
        for(int k = 0; k < numCols; k++){
            if(myMatrix[i].getRow() == j && myMatrix[i].getCol() == k){
                cout << myMatrix[i].getValue() << " ";
                i++;
            }
            else{
                cout << commonValue << " ";
            }
        }
        cout << endl;
    }
}

SparseMatrix* SparseMatrix::Transpose()
{
    SparseMatrix* transposedMatrix = new SparseMatrix(*this);
    for(int i= 0; i < numNonSparseValues; i++){
       int tempRow = transposedMatrix->myMatrix[i].getRow();
       int tempCol = transposedMatrix->myMatrix[i].getCol();
       transposedMatrix->myMatrix[i].setRow(tempCol);
       transposedMatrix->myMatrix[i].setCol(tempRow);
    }
    return transposedMatrix;
}

SparseMatrix* SparseMatrix::Add(SparseMatrix& M) {
        if (numRows != M.numRows || numCols != M.numCols) {
            cerr << "Matrix dimensions do not match for addition" << endl;
            return nullptr;
        }

        SparseRow* resultElements1 = new SparseRow[numCols * numRows];
        int i;
        int l;
        int resultSize;
        i = 0;
        l = 0;
        for(int j = 0; j < numRows; j++){
            for(int k = 0; k < numCols; k++){
                if(myMatrix[i].getRow() == j && myMatrix[i].getCol() == k){
                    resultElements1[l].setRow(j);
                    resultElements1[l].setCol(k);
                    resultElements1[l].setValue(myMatrix[i].getValue());
                    ++i;
                }
                else{
                    resultElements1[l].setRow(j);
                    resultElements1[l].setCol(k);
                    resultElements1[l].setValue(commonValue);
                }
                ++l;
            }
        }

        cout << "should be 900: " << resultElements1[3].getValue() << endl;

        SparseRow* resultElements2 = new SparseRow[M.numCols * M.numRows];
        i = 0;
        l = 0;
        for(int j = 0; j < M.numRows; j++){
            for(int k = 0; k < M.numCols; k++){
                if(M.myMatrix[i].getRow() == j && M.myMatrix[i].getCol() == k){
                    resultElements2[l].setRow(j);
                    resultElements2[l].setCol(k);
                    resultElements2[l].setValue(M.myMatrix[i].getValue());
                    ++i;
                }
                else{
                    resultElements2[l].setRow(j);
                    resultElements2[l].setCol(k);
                    resultElements2[l].setValue(commonValue);
                }
                ++l;
            }
        }
        cout << "should be 49 " << M.myMatrix[1].getValue() << endl;
        cout << "should be 49: " << resultElements2[4].getValue() << endl;
        SparseMatrix* result = new SparseMatrix(numRows, numCols, commonValue, numNonSparseValues);
        
        SparseRow* resultElements3 = new SparseRow[numCols * numRows];
        for(int j = 0; j < numCols * numRows; j++){
            resultElements3[j].setRow(resultElements1[j].getRow());
            resultElements3[j].setCol(resultElements1[j].getCol());
            resultElements3[j].setValue(resultElements1[j].getValue() + resultElements2[j].getValue());

        }

        cout << "should be 236: " << resultElements3[7].getValue() << endl;

        for(int j = 0; j < numCols * numRows; j++){
            if(resultElements3[j].getValue() != commonValue){
                ++resultSize;
            }
        }

        SparseRow* resultElements = new SparseRow[resultSize];
        i = 0;
        for(int j = 0; j < numCols * numRows; j++){
            if(resultElements3[j].getValue() != commonValue){
                resultElements[i].setRow(resultElements3[j].getRow());
                resultElements[i].setCol(resultElements3[j].getCol());
                resultElements[i].setValue(resultElements3[j].getValue());
                ++i;
            }
        }
         cout << resultElements[1].getValue() << endl;
        (*result).setMyMatrix(&resultElements);

        return result;
}

SparseMatrix* SparseMatrix::Multiply(SparseMatrix& M) {
        if (numCols != M.numRows) {
            cerr << "Matrix dimensions do not match for multiplication" << endl;
            return nullptr;
        }

        SparseRow* resultElements = new SparseRow[numNonSparseValues];
        for (int i = 0; i < numNonSparseValues; i++) {
            for(int j = 0; j < M.numCols; j++){
                if (myMatrix[i].getRow() == M.myMatrix[i].getRow() && M.myMatrix[i].getCol() == j) {
                    resultElements[i].setRow(myMatrix[i].getRow());
                    resultElements[i].setCol(M.myMatrix[i].getCol());
                    resultElements[i].setValue(myMatrix[i].getValue() * M.myMatrix[i].getValue()); 
                }
            }
        }
        SparseMatrix* result = new SparseMatrix(numRows, M.numCols, commonValue, numNonSparseValues);
        (*result).setMyMatrix(&resultElements);

 

        return result;
}



ostream& operator<<(ostream& s, SparseMatrix& sm)
{
    for (int i = 0; i < sm.numRows; i++)
    {
        for (int j = 0; j < sm.numCols; j++)
        {
            s << sm.myMatrix[i * sm.numCols + j];
        }
    }
    return s;
}

int main () {
   int rows, cols, commonValue, numNonSparseValues;
   cin >> rows >> cols >> commonValue >> numNonSparseValues;
   
   SparseMatrix* firstOne = new SparseMatrix(rows, cols, commonValue, numNonSparseValues);
   cout << "First one in sparse matrix format:" << endl;
   (*firstOne).displaySparseMatrix();
   
   cout << "After transpose:" << endl;
   (*(*firstOne).Transpose()).displaySparseMatrix();

   cout << "First one in matrix format:" << endl;
   (*firstOne).displayMatrix();
   
   cout << "Second one in sparse matrix format:" << endl;
   cin >> rows >> cols >> commonValue >> numNonSparseValues;
   
   SparseMatrix* secondOne = new SparseMatrix(rows, cols, commonValue, numNonSparseValues);
   (*secondOne).displaySparseMatrix();

    cout << "After transpose:" << endl;
    (*(*secondOne).Transpose()).displaySparseMatrix();

    cout << "Second one in matrix format:" << endl;
    (*secondOne).displayMatrix();

    cout << "Matrix addition result" << endl;
    SparseMatrix* result = (*firstOne).Add(*secondOne);
    (*result).displayMatrix();

   return 0;
}


