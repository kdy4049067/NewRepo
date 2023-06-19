#include <iostream>
#include <malloc.h>
#include<iomanip>
using namespace std;

// 2차원 배열 matrix 콘솔 출력 함수
void printMatrix(double** matrix)
{
	// row, col - 입력받은 2차원 배열 matrix의 행, 열의 갯수
	int row = _msize(matrix) / sizeof(double*);
	int col = _msize(matrix[0]) / sizeof(double);
	for (int r = 0; r < row; r++) {
		for (int c = 0; c < col; c++) {
			printf("%.2f\t", matrix[r][c]);
		}
		cout << '\n';
	}
}

// matrix의 row1번째 행과 row2번째 행을 교환
void rowSwitch(double** matrix, int row1, int row2) {
	double* temp = matrix[row1];
	matrix[row1] = matrix[row2];
	matrix[row2] = temp;
}

// matrix의 row번째 행에 coef를 곱함.
void rowMultiply(double** matrix, int row, double coef) {
	int col = _msize(matrix[0]) / sizeof(double);
	for (int i = 0; i < col; i++) {
		matrix[row][i] *= coef;
	}
}

// matrix의 dst_row번째 행에 src_row번째 행을 coef만큼 곱해서 더함. coef 미지정시 계수는 1
void rowAdd(double** matrix, int src_row, int dst_row, double coef = 1.0) {

	int col = _msize(matrix[0]) / sizeof(double);

	for (int i = 0; i < col; i++)
		matrix[dst_row][i] += coef * matrix[src_row][i];
}

//첫번째가 0이 아닌 열의 인덱스 리턴
int findNonZeroCol(double* row) {
	int col = _msize(row) / sizeof(double);
	for (int c = 0; c < col - 1; c++) {
		if (abs(row[c]) > 0.00000001) return c; // 부동소수점 연산 오차 보정
		else row[c] = 0;
		continue;
	}
	return -1;
}

// 0인행을 matrix의 가장 아래 행으로 내리고  0이 아닌 행은 다른 행의 같은 열과 값을 맞추어 빼줌
void echelonForm(double** matrix) {
	int row = _msize(matrix) / sizeof(double*);
	int col = _msize(matrix[0]) / sizeof(double);
	int zero_row_count = 0;

	for (int r = 0; r < row - zero_row_count - 1; r++) {
		int c = findNonZeroCol(matrix[r]); //r행에서 col이 0이아닌 열을 찾음
		if (c == -1) {
			rowSwitch(matrix, r, row - 1 - zero_row_count);//r행이 0 일때 그 행과 마지막행을 바꾼다.
			r--;
			zero_row_count++; //zero row count에 1더함
			continue;
		}
		else {
			for (int i = 0; i < row - 1 - r; i++) {
				rowAdd(matrix, r, 1 + r + i, (-1) * matrix[1 + r + i][c] / matrix[r][c]);
			}// 다른행의 열을 r행의 열로 나누어 r행에 곱해주고 값을 더해준다.
		}
	}
}

//echelonform을 구한상태에서 각 열의 첫 0이 아닌 숫자를 1로 만듦, 각 행의 1위의 수를 모두 0으로 만듦.
void reducedEchelonForm(double** matrix) {
	int row = _msize(matrix) / sizeof(double*);
	int col = _msize(matrix[0]) / sizeof(double);

	echelonForm(matrix); //echelonfrom으로 만들기

	for (int r = 0; r < row; r++) {
		int c = findNonZeroCol(matrix[r]);
		if (c == -1) continue;
		else {
			rowMultiply(matrix, r, 1 / matrix[r][c]);
			//r행의 값을 첫번째 0 이아닌 숫자가 있는 col의 역수로 나눈다
		}
	}
	for (int i = row - 1; i >= 0; i--) {
		int j = findNonZeroCol(matrix[i]);
		if (j == -1) continue;
		else {
			for (int k = 0; k < i; k++) {
				rowAdd(matrix, i, i - 1 - k, (-1) * matrix[i - 1 - k][j]);
				//위에 행으로 올라가면서 숫자가 있으면 계수를 맞추어 빼준다.
			}
		}
	}
}


void print_infinite_solution(double** matrix)
{
	int row = _msize(matrix) / sizeof(double*);
	int col = _msize(matrix[0]) / sizeof(double);
	int returnvalue = 0;//findNonZeroCol의 리턴값을 저장하는 변수
	int count = 0; //free변수 개수 카운터
	int  *check_freevalue;//free변수 위치 저장 행렬
	int* leadingvalue_col = new int[col - 1];//leading변수의 열번호를 담는 배열
	int* leadingvalue_row = new int[row];//leading변수의 행번호를 담는 배열
	int* freevalue_col = new int[col - 1];//free변수의 열번호를 담는 배열
	int* freevalue_row = new int[row];//free변수의 행번호를 담는 배열
	int retrun_findNonZeroCol = 0;//findNonzeroCol의 리턴값 저장변수
	int leadingvalue_counter = 0, freevalue_counter = 0;//free변수와 leading변수 원소의 개수 저장변수
	int min_freevalue_col = col - 1;//행렬 모든 원소중 free변수중 가장 작은 열의 열을 저장

	for (int i = 0; i < row; i++) {
		retrun_findNonZeroCol = findNonZeroCol(matrix[i]);
		if (retrun_findNonZeroCol == -1)//행이 전부 0인경우 다음행으로 넘어감
			continue;

		if (matrix[i][retrun_findNonZeroCol] == 1) { //leading value 위치저장
			leadingvalue_col[leadingvalue_counter] = retrun_findNonZeroCol;
			leadingvalue_row[leadingvalue_counter] = i;
			leadingvalue_counter++;
		}
		for (int j = retrun_findNonZeroCol + 1; j < col - 1; j++) { //free value 위치저장
			if (matrix[i][j] != 0) {
				freevalue_col[freevalue_counter] = j;
				freevalue_row[freevalue_counter] = i;
				freevalue_counter++;
			}
		}
	}

	cout << "Let->";
	int for_counter = 0; // for문 동작 카운터
	for (int i = min_freevalue_col; i < col - 1; i++)
	{
		for (int j = 0; j < freevalue_counter; j++)
		{
			if (freevalue_row[j] == leadingvalue_row[i]) { //해당 리딩변수행에 free변수가 있다고 하면(리딩변수의 행과 free변수의 행이 일치하면)

				if (matrix[freevalue_row[j]][freevalue_col[j]] < 0)
					cout << "+";
				if (matrix[freevalue_row[j]][freevalue_col[j]] != 0)
					cout << fixed << setprecision(2) << -matrix[freevalue_row[j]][freevalue_col[j]] << (char)(117 - freevalue_col[j] - (min_freevalue_col)+2);
			}
			cout << "t(" << min_freevalue_col + 1 + for_counter << ") = " << char(117 - for_counter) << "  ";
			for_counter++;
		}
		cout << endl;

		cout << "\n\n\n";
	}

	for (int i = 0; i < row; i++) //맨앞 행부터 읽기
	{
		returnvalue = findNonZeroCol(matrix[i]);
		if (returnvalue == -1)//읽은 모든 열이 0이면
			continue; //다음 행으로


		else { //나머지의 경우
			check_freevalue = new int[col - 1];
			for (int j = returnvalue + 1; j < col - 1; j++) {
				if (matrix[i][j] != 0) { //free변수가 있으면
					check_freevalue[count] = j;//해당 열번호를 담기
					count++;
				}
			}
			//행의 free변수가 없으면 이것만 실행
			cout << "t(" << returnvalue + 1 << ") = " << fixed << setprecision(2) << matrix[i][col - 1];

			if (count > 0) {//행의 free변수가 있으면

				int p = 0; //check_freevalue의 위치를 1개씩 증가하여 모든 free변수 꺼내기 
				for (int j = returnvalue + 1; j < col - 1; j++) { //리딩변수 뒤에 부터 출력 시작
					if (matrix[i][j] == 0) // 값이 0인경우는 출력 x
						continue;
					else {
						int freevalue_col = check_freevalue[p];
						if (matrix[i][freevalue_col] <= 0)
							cout << "+";

						cout << fixed << setprecision(2) << -(matrix[i][check_freevalue[p]]) << "t(" << check_freevalue[p] + 1 << ")";
						p++;
					}

				}
				for (int i = 0; i < p; i++) {
					if (check_freevalue[i] < min_freevalue_col)
						min_freevalue_col = check_freevalue[i];
				}
				cout << endl;
			}
			count = 0;//카운트 초기화
		}
	}

}



void printOnlySolution(double **matrix) {
	int returnvalue = 0;
	int row = _msize(matrix) / sizeof(double*);
	int col = _msize(matrix[0]) / sizeof(double);
	bool free = false;
	for (int r = 0; r < row; r++) {
		returnvalue = findNonZeroCol(matrix[r]);
		if (returnvalue == -1)
			break;
		else {
			for (int j = returnvalue + 1; j < col - 1; j++) {
				if (matrix[r][j] == 0)//free변수가 없으면
				{
					free = false;
					continue;
				}
				else {
					free = true;
					break;
				}
			}
		}
		if (free == false)
			continue;
		else if (free == true)
			break;
	}
	if (free == false)
		for (int j = 0; j < row; j++)
			cout << "x" << j + 1 << " = " << matrix[j][col - 1] << "\n";
}


void print_infinite(double **matrix) {

	int row = _msize(matrix) / sizeof(double*);
	int col = _msize(matrix[0]) / sizeof(double);
	int* leadingvalue_col = new int[col - 1];//leading변수의 열번호를 담는 배열
	int* leadingvalue_row = new int[row];//leading변수의 행번호를 담는 배열
	int* freevalue_col = new int[col - 1];//free변수의 열번호를 담는 배열
	int* freevalue_row = new int[row];//free변수의 행번호를 담는 배열
	int retrun_findNonZeroCol = 0;//findNonzeroCol의 리턴값 저장변수
	int leadingvalue_counter = 0, freevalue_counter = 0;//free변수와 leading변수 원소의 개수 저장변수
	int min_freevalue_col = col - 1;//행렬 모든 원소중 free변수중 가장 작은 열의 열을 저장

	for (int i = 0; i < row; i++) {
		retrun_findNonZeroCol = findNonZeroCol(matrix[i]);
		if (retrun_findNonZeroCol == -1)//행이 전부 0인경우 다음행으로 넘어감
			continue;

		if (matrix[i][retrun_findNonZeroCol] == 1) { //leading value 위치저장
			leadingvalue_col[leadingvalue_counter] = retrun_findNonZeroCol;
			leadingvalue_row[leadingvalue_counter] = i;
			leadingvalue_counter++;
		}
		for (int j = retrun_findNonZeroCol + 1; j < col - 1; j++) { //free value 위치저장
			if (matrix[i][j] != 0) {
				freevalue_col[freevalue_counter] = j;
				freevalue_row[freevalue_counter] = i;
				freevalue_counter++;
			}
		}
	}

	for (int i = 0; i < freevalue_counter; i++) { //free변수중 가장 작은 열의 열을 저장
		if (freevalue_col[i] < min_freevalue_col)
			min_freevalue_col = freevalue_col[i];
	}

	//free변수의 개수는 col-1을 초과할 수 없음!
	for (int i = 0; i < leadingvalue_counter; i++) { //출력문
		cout << "t(" << leadingvalue_col[i] + 1 << ") = " << matrix[leadingvalue_row[i]][col - 1]; //리딩변수출력 및 b(Ax=b의 b)값 출력 

		for (int j = 0; j < freevalue_counter; j++)
		{
			if (freevalue_row[j] == leadingvalue_row[i]) { //해당 리딩변수행에 free변수가 있다고 하면(리딩변수의 행과 free변수의 행이 일치하면)

				if (matrix[freevalue_row[j]][freevalue_col[j]] < 0)
					cout << "+";
				if (matrix[freevalue_row[j]][freevalue_col[j]] != 0)
					cout << fixed << setprecision(2) << -matrix[freevalue_row[j]][freevalue_col[j]] << "t(" << freevalue_col[j] + 1 << ")";
			}
		}
		cout << endl;
	}

	cout << "\n\n\n";
	cout << "Let->";
	int for_counter = 0; // for문 동작 카운터
	for (int i = min_freevalue_col; i < col - 1; i++)
	{
		for (int j = 0; j < freevalue_counter; j++)
		{

		}
		cout << "t(" << min_freevalue_col + 1 + for_counter << ") = " << char(117 - for_counter) << "  ";
		for_counter++;
	}
	cout << endl;

	for (int i = 0; i < leadingvalue_counter; i++) {
		cout << "t(" << leadingvalue_col[i] + 1 << ") = " << matrix[leadingvalue_row[i]][col - 1];
		for (int j = 0; j < freevalue_counter; j++)
		{
			if (freevalue_row[j] == leadingvalue_row[i]) { //해당 리딩변수행에 free변수가 있다고 하면(리딩변수의 행과 free변수의 행이 일치하면)

				if (matrix[freevalue_row[j]][freevalue_col[j]] < 0)
					cout << "+";
				if (matrix[freevalue_row[j]][freevalue_col[j]] != 0)
					cout << fixed << setprecision(2) << -matrix[freevalue_row[j]][freevalue_col[j]] << (char)(117 - freevalue_col[j] - (min_freevalue_col)+2);
			}
		}
		cout << endl;
	}
}


int main() {
	int row, col;
	cout << "연립 방정식의 식 개수를 입력하시오: ";
	cin >> row;
	cout << "변수 개수를 입력 하시오:";
	cin >> col;

	double** matrix = new double*[row];
	for (int i = 0; i < row; i++)
		matrix[i] = new double[col + 1];

	cout << "Augmented 행렬의 각 행의 내용을 입력하시오\n";
	for (int r = 0; r < row; r++) {
		cout << r + 1 << "행: ";
		for (int c = 0; c < col + 1; c++) {
			cin >> matrix[r][c];
		}
	}

	cout << "--------" << endl;

	reducedEchelonForm(matrix);

	int zero_row_counter = 0;
	bool isConsistent = true;

	for (int i = 0; i < row; i++) {
		if (findNonZeroCol(matrix[i]) == -1) {
			zero_row_counter++; //0인행이 있으면 + 1
			if (matrix[i][col] != 0) isConsistent = false;
		}
	}
	if (isConsistent == false) cout << "해가 존재하지 않음" << endl;

	else if (isConsistent == true && row - zero_row_counter == col) {
		printMatrix(matrix);
		printOnlySolution(matrix);
	}
	else if (isConsistent == true && row - zero_row_counter < col) {
		printMatrix(matrix);
		print_infinite(matrix);
	}
	cout << "\n" << "zero_row_counter : " << zero_row_counter << "\n" << "isConsistent: " << (isConsistent ? "True" : "False") << endl;

	for (int i = 0; i < row; i++) {
		delete[] matrix[i];
	}
	delete[] matrix;


	return 0;
}