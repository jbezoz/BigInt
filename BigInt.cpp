#include <iostream>
#include <string.h>

//Each Object of BigInt class store sign, size, pointer to first element of BigInt
class BigInt
{
public:													//all data members are public as they have to be accesed and changed by functions and user
	int sign;
	int size;
	char* elem;											// elements are stored in char array

	BigInt() {}											//default constructor need to be initialized to assign variable at any point in time

	BigInt(const char* A)								//parameterized constructor to assign values to the object (const char * to get variable size string >128bytes / >1024bits)
	{
		elem = (char*)A;
		sign = strstr(A, "-") != NULL ? 1 : 0;			//check for "-" sign and assign 1 for negative else 0 for positive
		size = strlen(A);
		if (sign == 1)									//if negative increment the pointer by 1 and decrement the size by 1
		{
			elem = &elem[1];
			size--;
		}
	}
};

//Greater magnitude : 1 -> A , 2 -> B , 0 -> same
int compare_Mag(BigInt A, BigInt B)
{
	int ans = 0;
	if (A.size > B.size)					//compare size and assign 1 if A is greater , 2 if A is smaller 
	{
		ans = 1;
	}
	else if (B.size > A.size)
	{
		ans = 2;
	}
	else									//if size are same it loops over the entire array and compare each elem if any greater element is incountered assign 1,2 
	{										// else at the end assign 0 for same elements (which is the default value of ans)
		int i = 0;
		while (i < A.size)
		{
			if (A.elem[i] > B.elem[i])
			{
				ans = 1;
				i = A.size;
			}
			else if (B.elem[i] > A.elem[i])
			{
				ans = 2;
				i = A.size;
			}
			i++;
		}
	}
	return ans;
}

BigInt substraction_BigInt(BigInt A, BigInt B); 		//forward declaration as it is called in addition_BigInt()

BigInt addition_array(BigInt A, BigInt B)
{
	BigInt C;														// Object of Class BigInt is generated to return the size sign and elements after addition

	int term;														//added using Standard Addition algorithm (i.e. adding each digit with same place values) and the corresponding carry
	int carry = 0;													// generated to next digit with higher place value (higher index in char array)
	int size_A = A.size - 1;
	int size_B = B.size - 1;
	int size_C = (A.size > B.size) ? A.size + 1 : B.size + 1;
	int size_C1 = size_C - 1;
	char* C1_temp;
	C1_temp = (char*)malloc(size_C1 * sizeof(char));				//temporary pointer is assigned with size of greater integer as the addition cannot exceed +1 more than greater integer

	while (size_A >= 0 && size_B >= 0)
	{
		term = (int)(A.elem[size_A--]) - 48 + (int)(B.elem[size_B--]) - 48 + carry;
		C1_temp[size_C1--] = (char)((term % 10) + 48);
		carry = term / 10;
	}

	while (size_A >= 0)
	{																// each digit is assigned to temp array after typecasting and adding 48 (i.e. ASCII value of '0' character)
		term = (int)(A.elem[size_A--]) - 48 + carry;
		C1_temp[size_C1--] = (char)((term % 10) + 48);
		carry = term / 10;
	}

	while (size_B >= 0)
	{
		term = (int)(B.elem[size_B--]) - 48 + carry;
		C1_temp[size_C1--] = (char)((term % 10) + 48);
		carry = term / 10;
	}

	if (carry == 1)													// If carry is generated at the end it is assigned to the 0 th index 
	{
		C1_temp[0] = '1';
		C.elem = C1_temp;
		C.size = size_C;
	}
	else															// If carray is not generated pointer is incremented to next position
	{
		C.elem = &C1_temp[1];
		C.size = size_C - 1;
	}

	return C;
}

BigInt addition_BigInt(BigInt A, BigInt B)
{
	BigInt C;
	if (A.sign == B.sign)
	{
		if (compare_Mag(A, B) == 1 || compare_Mag(A, B) == 0)
		{
			C = addition_array(A, B);
		}
		else
		{
			C = addition_array(B, A);
		}
		C.sign = A.sign;										//if signs are same any input sign is valid output
	}
	else														//if sign are opposite substraction_BigInt() is called
	{
		if (compare_Mag(A, B) == 1)
		{
			int temp = A.sign;								// needed to store sign tempo and change sign to work with substraction_BigInt() function 
			A.sign = B.sign;
			C = substraction_BigInt(A, B);
			C.sign = temp;									// sign of greater magnitude integer is the output 
		}
		else if (compare_Mag(A, B) == 2)
		{
			int temp = B.sign;
			A.sign = B.sign;
			C = substraction_BigInt(B, A);
			C.sign = temp;
		}
		else if (compare_Mag(A, B) == 0)
		{
			char zero[] = { '0' };								// opposite sign, same magnitude computation is saved and 0 is output
			C.elem = zero;
			C.size = 1;
			C.sign = 0;
		}
	}

	return C;
}

BigInt substraction_array(BigInt A, BigInt B)								//substraction_array() can only substract two positive integer A, B ; A > B (magnitude)
{
	BigInt C;
	char* a = A.elem;
	char* b = B.elem;

	int i, j, k, borr, size1 = 0, size2 = 0;
	int* diff;
	diff = (int*)malloc(A.size * sizeof(int));								// Assiging dynamic memory to store the inputs and result	
	int* x;
	x = (int*)malloc(B.size * sizeof(int));
	int* y;
	y = (int*)malloc(A.size * sizeof(int));
	char* temp;
	temp = (char*)malloc((A.size + 1) * sizeof(char));

	for (i = 0; i < A.size; i++)											//converting to int data type to facilitate calculation instead of (+/-) ,  ('0'/48) to the array elements
	{
		x[i] = a[i] - '0';	//conversion into int array 
		size1++;
	}

	for (i = 0; i < B.size; i++)
	{
		y[i] = b[i] - '0';	//conversion into int array 
		size2++;
	}

	for (j = 0; j < size2; j++)												//substract using Standard Substraction Algorithm (i.e. by adding -1 to the next digit from right to left
	{																		// if A(digit) < B(digit)
		y[j] = -y[j];
	}

	k = borr = 0;

	for (i = size1 - 1, j = size2 - 1; i >= 0 && j >= 0; i--, j--, k++)
	{
		diff[k] = x[i] + y[j] - borr;
		if (diff[k] < 0)
		{
			diff[k] += 10;
			borr = 1;
		}
		else
			borr = 0;
	}

	if (i >= 0)
	{
		for (; i >= 0; i--, k++)
		{
			diff[k] = x[i] - borr;
			if (diff[k] < 0)
			{
				diff[k] += 10;
				borr = 1;
			}
			else
				borr = 0;
		}
	}

	i = j = 0;
	for (i = k - 1; i >= 0; i--)
	{
		if (diff[i] < 0)
		{
			temp[j++] = '-';
			temp[j++] = -(diff[i]) + '0';
		}
		else
			temp[j++] = diff[i] + '0';
	}

	temp[j] = '\0';
	C.elem = temp;
	for (i = 0; temp[i] != '\0'; i++) {}

	C.size = i;

	i = 0;																					//eliminating zeros' except last zero by 
	int changed = 0;
	while (i < C.size - 1)
	{
		if (C.elem[i] == '0')
		{
			i++;
			changed = 1;																	//keeping track by changed variable 1,0 values
		}
		else
		{
			break;
		}
	}
	if (changed == 1)
	{
		C.elem = &C.elem[i];																//reassiging pointer if changed == 1
		C.size = C.size - i + 1;															//reassigning size as pointer is moved
	}

	return C;
}

BigInt substraction_BigInt(BigInt A, BigInt B)
{
	BigInt A1, B1, C;
	A1 = A;
	B1 = B;
	A1.sign = 0;
	B1.sign = 0;
	//Magnitude Calculation
	if (A.sign == B.sign)					//assigning sign initially as they input magnitude amd sign dependent 
	{
		if (compare_Mag(A, B) == 1)
		{
			C = substraction_array(A1, B1); //need to take care of big number && check A B / A1 B1
		}
		else if (compare_Mag(A, B) == 2)
		{
			C = substraction_array(B1, A1); //reversing the input to use substraction_array() function
		}
		else if (compare_Mag(A, B) == 0) // explicity modified C to 0 to avoid using the substraction_array() if both have same size and ++ sign
		{
			char zero[] = { '0' };
			C.elem = zero;
			C.size = 1;
			C.sign = 0;
		}
	}
	else
	{
		C = addition_BigInt(A1, B1);		//if sign are opposite addition_BigInt() is called
	}

	//assuming deafult C.sign = 0 (i.e. in case both are same number and result = 0)
	//result sign is pre-determined
	if (compare_Mag(A, B) == 1)
	{
		C.sign = A.sign;
	}
	else if (compare_Mag(A, B) == 2)
	{
		if (A.sign == 1 && B.sign == 1) { C.sign = 0; }
		else if (A.sign == 1 && B.sign == 0) { C.sign = 1; }
		else if (A.sign == 0 && B.sign == 1) { C.sign = 0; }
		else { C.sign = 1; }
	}

	return C;
}

BigInt multiplication_array(BigInt A, BigInt B)
{
	BigInt C;

	char* C1_temp;
	C1_temp = (char*)malloc((A.size + B.size)*sizeof(char));                        
	for (int k = 0; k < A.size + B.size; k++) { C1_temp[k] = C1_temp[k] + '0'; } //max space should alwys be A.size + B.size (not more)
	int count = A.size + B.size - 1;
	int j;
	int l = B.size - 1;
	int size_B = B.size;

	while (size_B--)
	{
		j = count;
		int carry = 0;
		char* temp;
		temp = (char*)malloc(A.size + 1*sizeof(char)); 
		int temp_size = A.size+1;
		for (int i = A.size - 1; i >= 0; i--)
		{
			int store1 = (int)(B.elem[l] - '0') * (int)(A.elem[i] - '0') + carry;     // temp pointer stores the intermediate multiplication value in char array
			temp[i + 1] = (char)((store1 % 10) + '0');								// And adds to the base array of size A + B.
			carry = store1 / 10;
		}

		if (carry != 0)
		{
			temp[0] = (char)(carry + '0');											// If carry is generated it is added to the first positon of temp/intermediate multip array
		}
		else
		{
			temp = &temp[1];														// else pointer is incremented as an additional space for left to accommodate carry
		}

		temp_size = (carry != 0) ? temp_size + 1 : temp_size;
		carry = 0;
		for (int i = temp_size - 2; i >= 0; i--)
		{
			int store2 = (int)(C1_temp[j] - '0') + (int)(temp[i] - '0') + carry;	// the intermediate multip array is added to the base array which store intermediate final answer
			C1_temp[j] = (char)(store2 % 10) + '0';
			carry = store2 / 10;
			j--;
		}
		(carry != 0) ? (C1_temp[j] = (char)(carry + '0')) : 0;					//after completion of loop base array stores the result and carry is accommodated
		count--;
		l--;
	}

	//eliminating zeros if carry isn't generated
	if (C1_temp[0] == '0')
	{
		C1_temp = &C1_temp[1];
		C.elem = C1_temp;
		C.size = A.size + B.size - 1;											// reassign the position of pointer based on carry generated
	}
	else
	{
		C.elem = C1_temp;
		C.size = A.size + B.size;
	}

	return C;

}

BigInt multiplication_BigInt(BigInt A, BigInt B)
{
	BigInt C;

	//assigning magnitude
	if (compare_Mag(A, B) == 2)
	{
		C = multiplication_array(B, A);
	}
	else
	{
		C = multiplication_array(A, B);
	}
	//assigning sign
	if (A.sign == B.sign)
	{
		C.sign = 0;
	}
	else
	{
		C.sign = 1;
	}

	return C;
}

void print(BigInt C)
{
	(C.sign == 1) ? printf("-") : 0;       // assign '-' if sign == 1 and print the BigInt elements using pointer and size from the class instances/objects
	for (int i = 0; i < C.size; i++)
	{
		std::cout << C.elem[i];
	}
}

int main()
{
	BigInt A("123456789"), B("-987654321");
	BigInt C, D, E;
	C = addition_BigInt(A, B);
	D = substraction_BigInt(A, B);
	E = multiplication_BigInt(A, B);
	std::cout << "Addition : ";
	print(C);
	std::cout << "\n";
	std::cout << "Substraction : ";
	print(D);
	std::cout << "\n";
	std::cout << "Multiplication : ";
	print(E);
	std::cout << "\n";
}