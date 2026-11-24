#include <iostream>
#include <vector>
#include <deque>
#include <cstdlib>

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

void fillVec(std::vector<int> &vec, std::deque<int> &deq, char **av, int ac)
{
    for (int i = 1; i < ac; ++i)
    {
        int value = std::atoi(av[i]); // validation à faire avant la conversion
        vec.push_back(value);
        deq.push_back(value);
    }
}

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

void printCont(const std::vector<int> &vec)
{
    std::vector<int>::const_iterator it = vec.begin();
    while(it != vec.end())
    {
        std::cout << *it << " ";
        it++;
    }
    std::cout << std::endl;
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

void merge(std::vector<int> &leftArray, std::vector<int> &rightArray, std::vector<int> &array)
{
    int leftSize = leftArray.size();
    int rightSize = rightArray.size();
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

void mergeSortCont(std::vector<int> &array)
{
    int length = array.size();
    if (length <= 1)
        return;

    int middle = length / 2;
    std::vector<int> leftArr(middle);
    std::vector<int> rightArr(array.size() - middle);

    int r = 0;
    int l = 0;


    for (; l < length; l++)
    {

        if (l < middle)
        {

            leftArr[l] = array[l];
        }
        else
        {
            rightArr[r] = array[l];
            r++;
        }
    }

    mergeSortCont(leftArr);
    mergeSortCont(rightArr);
    merge(leftArr, rightArr, array);
}

void insertionSort(std::vector<int> &vec)
{
    for (int i = 1; i < vec.size(); i++)
    {
        int temp = vec[i];
        int j = i - 1;

        while(j >= 0 && vec[j] > temp)
        {
            vec[j + 1] = vec[j];
            j--;
        }
        vec[j + 1] = temp;
    }
}

int makePair(std::vector<int> &vec, std::vector<int> &small, std::vector<int> &large)
{
    int s = 1;
    int straggler = -1;
    int t = 0;
    if ((vec.size() % 2) == 1)
    {
        s = 2;
        straggler = vec[0];
    }
    for (int i = s; i < vec.size(); i += 2)
    {
        int j = i - 1;
        if (vec[i] > vec[j])
        {
            large[t] = vec[i];
            small[t] = vec[j];
        }
        else
        {
            large[t] = vec[j];
            small[t] = vec[i];
        }
        t++;
    }
    return (straggler);
}

void sortPair(std::vector<int> &small, std::vector<int> &large)
{
    for (int i = 0; i < large.size(); i++)
    {
        int tempS = small[i];
        int tempL = large[i];
        int j = i - 1;

        while(j >= 0 && large[j] > tempL)
        {
            small[j + 1] = small[j];
            large[j + 1] = large[j];
            j--;
        }
        small[j + 1] = tempS;
        large[j + 1] = tempL;
    }
}

void printPair(std::vector<int> &small, std::vector<int> &large)
{
    for (int i = 0; i < small.size(); i++)
    {
        std::cout << "small : " << small[i] << " ";
        std::cout << "large : " << large[i] << " ";
        std::cout << std::endl;
    }
}

void mergeInsert(std::vector<int> &vec)
{
    if (vec.size() <= 1)
        return;

    std::vector<int> small((vec.size()/2));
    std::vector<int> large((vec.size()/2));

    int straggler = makePair(vec, small, large);

    printPair(small, large);
    sortPair(small, large);
    printPair(small, large);

}

void mergeInsertV2(std::vector<int> &vec)
{
    

}

int main(int ac, char **av)
{
    std::vector<int> vec;
    std::vector<int> vec2;
    std::deque<int> deq;

    fillVec(vec, deq, av, ac);
    //fillVec(vec2, deq, av, ac);
    //printCont(vec);

    // std::cout << std::endl;
    // std::cout << "==== Merge Sort ====" << std::endl;
    // std::cout << std::endl;

    // //mergeSortCont(vec);
    // //printCont(vec);

    // std::cout << std::endl;
    // std::cout << "==== Insert Sort ====" << std::endl;
    // std::cout << std::endl;

    //insertionSort(vec2);

    //printCont(vec2);

    std::cout << std::endl;
    std::cout << "==== Merge Insert Sort ====" << std::endl;
    std::cout << std::endl;

    mergeInsert(vec);

    return (0);
}