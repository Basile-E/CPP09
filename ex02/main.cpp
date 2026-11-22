#include <iostream>

/*

 - Your program must be able to use a positive integer sequence as an argument
 
 - If an error occurs during program execution, an error message should be displayed
on the standard error.

 - You must use at least two different containers in your code to
validate this exercise. Your program must be able to handle at
least 3000 different integers.

 - Your program must use the merge-insert sort algorithm to sort the positive integer
sequence.

*/

int size(int *array)
{
    int s = 0;
    while (array[s])
        s++;
    return (11);
}

void print(int *arr)
{
    int len = size(arr);
    int i = 0;
    while(i < len)
    {
        std::cout << arr[i] << std::endl;
        i++;
    }
}

void merge(int *leftArray, int *rightArray, int *array)
{
    int leftSize = size(leftArray);
    int rightSize = size(rightArray);
    int i = 0;
    int r = 0;
    int l = 0;

    while (l < leftSize && r < rightSize)
    {
        if (leftArray[l] < rightArray[r])
        {
            array[i] = leftArray[l];
            i++;
            l++;
        }
        else
        {
            array[i] = rightArray[r];
            i++;
            r++;
        }
    }
    while (l < leftSize)
    {
        array[i] = leftArray[l];
        i++;
        l++;
    }
    while (r < rightSize)
    {
        array[i] = rightArray[r];
        i++;
        r++;
    }

}

void mergeSort(int *array)
{
    int length = size(array);
    if (length <= 1)
        return;

    int middle = length / 2;
    int leftArr[middle];
    int rightArr[length - middle];

    int r = 0;
    int l = 0;

    for (; l < length; l++)
    {
        if (l <= middle)
        {
            leftArr[l] = array[l];
        }
        else
        {
            rightArr[r] = array[l];
            r++;
        }
    }

    // std::cout << std::endl;
    // print(array);
    // std::cout << std::endl;
    mergeSort(leftArr);
    mergeSort(rightArr);
    merge(leftArr, rightArr, array);
}

int main(int ac, char **av)
{
    int arr[] = {13, 31, 4, 2131, 42, 7, 5, 89, 60, 120, 2};

    print(arr);
    std::cout << std::endl;
    mergeSort(arr);
    std::cout << std::endl;
    print(arr);

    return (0);
}