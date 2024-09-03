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
        SparseRow* originalMatrix;
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
        SparseRow* getOriginalMatrix();
};

SparseMatrix::SparseMatrix()
{
    numRows = 0;
    numCols = 0;
    commonValue = 0;
    numNonSparseValues = 0;
    myMatrix = NULL;
    originalMatrix = NULL;
}

SparseMatrix::SparseMatrix(int rows, int cols, int cv, int numNSV)
{
    numRows = rows;
    numCols = cols;
    commonValue = cv;
    numNonSparseValues = numNSV;
    myMatrix = new SparseRow[numNonSparseValues];
    originalMatrix = new SparseRow[rows * cols];

    if (myMatrix == nullptr) {
        cerr << "Memory allocation failed for myMatrix." << endl;
        return;
    }

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

    i = 0;
    int l = 0;
     for(int j = 0; j < numRows; j++){
        for(int k = 0; k < numCols; k++){
            if(myMatrix[i].getRow() == j && myMatrix[i].getCol() == k){
                originalMatrix[l].setRow(j);
                originalMatrix[l].setCol(k);
                originalMatrix[l].setValue(myMatrix[i].getValue());
                ++i;
            }
            else{
                originalMatrix[l].setRow(j);
                originalMatrix[l].setCol(k);
                originalMatrix[l].setValue(commonValue);
            }
            ++l;
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

// SparseRow* SparseMatrix::getOriginalMatrix() {
//     SparseRow* originalMatrix = new SparseRow[numNonSparseValues];
//     int i = 0;
//     int l = 0;
//      for(int j = 0; j < numRows; j++){
//         for(int k = 0; k < numCols; k++){
//             if(myMatrix[i].getRow() == j && myMatrix[i].getCol() == k){
//                 originalMatrix[l].setRow(j);
//                 originalMatrix[l].setCol(k);
//                 originalMatrix[l].setValue(myMatrix[i].getValue());
//                 ++i;
//             }
//             else{
//                 originalMatrix[l].setRow(j);
//                 originalMatrix[l].setCol(k);
//                 originalMatrix[l].setValue(commonValue);
//             }
//             ++l;
//         }
//     }
//     // cout << "Should be 0: "<< originalMatrix[10].getValue() << endl;
//     return originalMatrix;
// }

void SparseMatrix::displaySparseMatrix()
{
   for(int i = 0; i < numNonSparseValues; i++){
       myMatrix[i].display();
   }
}

void SparseMatrix::displayMatrix(){
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

        SparseRow* firstOriginalMatrix = originalMatrix;;
        SparseRow* secondOriginalMatrix = M.originalMatrix;

        SparseMatrix* result = new SparseMatrix(numRows, numCols, commonValue, numNonSparseValues);
        SparseRow* resultElements = new SparseRow[numCols * numRows];
        int resultSize = 0;
        int i; 

        
        for(int j = 0; j < numCols * numRows; j++){
            resultElements[j].setRow(firstOriginalMatrix[j].getRow());
            resultElements[j].setCol(firstOriginalMatrix[j].getCol());
            resultElements[j].setValue(firstOriginalMatrix[j].getValue() + secondOriginalMatrix[j].getValue());
        }

        for(int j = 0; j < numCols * numRows; j++){
            if(resultElements[j].getValue() != commonValue){
                ++resultSize;
            }
        }

        SparseRow* resultElementsSparse = new SparseRow[resultSize];
        
        i = 0;
        for(int j = 0; j < numCols * numRows; j++){
            if(resultElements[j].getValue() != commonValue){
                resultElementsSparse[i].setRow(resultElements[j].getRow());
                resultElementsSparse[i].setCol(resultElements[j].getCol());
                resultElementsSparse[i].setValue(resultElements[j].getValue());
                ++i;
            }
        }

        (*result).setMyMatrix(&resultElements);

        return result;
}

SparseMatrix* SparseMatrix::Multiply(SparseMatrix& M) {
    SparseRow* resultElements = new SparseRow[numRows * M.numCols];
    SparseRow* inputCol;
    SparseRow* inputRow;
    SparseRow* sparseResultElement;
    SparseMatrix* result;


    int resultSize = 0;

    int staticIndex = 0;
   
    int resultIndex = 0;
    int inputColIndex = 0;
    int colIndex = 0;
    int resultColIndex = 0;

    int inputRowIndex = 0;
    int rowIndex = 0;
    int sum = 0;

    if (numCols != M.numRows) {
            cerr << "Matrix dimensions do not match for multiplication" << endl;
            return nullptr;
    }

    for(rowIndex = 0;  rowIndex < numRows; ++rowIndex){
        //multiplier
        inputRow = new SparseRow[numCols];
        inputRowIndex = 0;
        for(int j = 0; j < numCols * numRows; j++){
            if(originalMatrix[j].getRow() == rowIndex){
                inputRow[inputRowIndex] = originalMatrix[j];
                ++inputRowIndex;
            }
        }
        //getting multiplied
         inputCol = new SparseRow[M.numRows];
        for(int k = 0; k < M.numCols; ++k){
            inputColIndex = 0;
            for(int j = 0; j < M.numCols * M.numRows; j++){
                if(M.originalMatrix[j].getCol() == k){
                    inputCol[inputColIndex] = M.originalMatrix[j];
                    ++inputColIndex;
                }      
            }
            //doing the multiplying
            for(int j = 0; j < numCols; ++j){
                sum += inputRow[j].getValue() * inputCol[j].getValue();
            }

            resultElements[resultIndex].setRow(rowIndex);
            resultElements[resultIndex].setCol(k);
            resultElements[resultIndex].setValue(sum); 
            ++resultIndex;
            sum = 0;
        }
        delete[] inputRow;
        delete[] inputCol;
    }

    for(int j = 0; j < numCols * M.numRows; j++){
        if(resultElements[j].getValue() != commonValue){
            ++resultSize;
        }
    }
    sparseResultElement = new SparseRow[resultSize];

    int i = 0;
    for(int j = 0; j < numCols * M.numRows; j++){
        if(resultElements[j].getValue() != commonValue){
            sparseResultElement[i].setRow(resultElements[j].getRow());
            sparseResultElement[i].setCol(resultElements[j].getCol());
            sparseResultElement[i].setValue(resultElements[j].getValue());
            ++i;
        }
    }

    result = new SparseMatrix(numRows, M.numCols, commonValue, numNonSparseValues);
    (*result).setMyMatrix(&sparseResultElement);

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

    cout << "Matrix multiplication result" << endl;
    SparseMatrix* result2 = (*firstOne).Multiply(*secondOne);
    (*result2).displayMatrix();

   return 0;
}


