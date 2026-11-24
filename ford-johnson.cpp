
#include <algorithm>
#include <iostream>
#include <vector>
#include <deque>

struct Item
{
    int value;
    size_t id;

    Item(int v = 0, size_t i = 0) : value(v), id(i) {}
};

struct Pair
{
    Item small;
    Item large;

    Pair(Item s, Item l) : small(s), large(l) {}
};

// Retourne l'ordre d'insertion des b2, b3, ...
std::vector<size_t> jacobsthalOrderV(size_t count)
{
    std::vector<size_t> order;
    size_t previous = 1;
    size_t current = 3;

    while (previous < count)
    {
        size_t end = std::min(current, count);

        for (size_t i = end; i > previous; --i)
            order.push_back(i - 1);

        size_t next = current + 2 * previous;
        previous = current;
        current = next;
    }
    return order;
}

// Retourne l'ordre d'insertion des b2, b3, ...
std::deque<size_t> jacobsthalOrderD(size_t count)
{
    std::deque<size_t> order;
    size_t previous = 1;
    size_t current = 3;

    while (previous < count)
    {
        size_t end = std::min(current, count);

        for (size_t i = end; i > previous; --i)
            order.push_back(i - 1);

        size_t next = current + 2 * previous;
        previous = current;
        current = next;
    }
    return order;
}

std::vector<Item> fordJohnsonItems(std::vector<Item> arr)
{
    if (arr.size() <= 1)
        return arr;

    // 1. Former les paires
    std::vector<Pair> pairs;

    for (size_t i = 0; i + 1 < arr.size(); i += 2)
    {
        Item a = arr[i];
        Item b = arr[i + 1];

        if (a.value > b.value)
            std::swap(a, b);

        pairs.push_back(Pair(a, b));
    }

    bool hasOdd = (arr.size() % 2 != 0);
    Item odd = hasOdd ? arr.back() : Item();

    // 2. Extraire les maximums
    std::vector<Item> large;

    for (size_t i = 0; i < pairs.size(); ++i)
        large.push_back(pairs[i].large);

    // 3. Récursion : trier uniquement les maximums
    large = fordJohnsonItems(large);

    // 4. Réassocier les petits éléments
    std::vector<Item> small;

    for (size_t i = 0; i < large.size(); ++i)
    {
        for (size_t j = 0; j < pairs.size(); ++j)
        {
            if (pairs[j].large.id == large[i].id)
            {
                small.push_back(pairs[j].small);
                break;
            }
        }
    }

    // 5. Construire la main chain
    std::vector<Item> main;
    main.push_back(small[0]);

    for (size_t i = 0; i < large.size(); ++i)
        main.push_back(large[i]);

    // 6. Insérer les small restants
    // L'élément impair participe aussi à Jacobsthal
    size_t count = small.size() + (hasOdd ? 1 : 0);
    std::vector<size_t> order = jacobsthalOrderV(count);

    for (size_t k = 0; k < order.size(); ++k)
    {
        size_t index = order[k];
        bool isOdd = (index == small.size());

        Item value = isOdd ? odd : small[index];

        // 7. Trouver la borne de recherche
        std::vector<Item>::iterator bound = main.end();

        if (!isOdd)
        {
            for (std::vector<Item>::iterator it = main.begin();
                 it != main.end(); ++it)
            {
                if (it->id == large[index].id)
                {
                    bound = it;
                    break;
                }
            }
        }

        // Recherche dichotomique bornée
        std::vector<Item>::iterator first = main.begin();
        std::vector<Item>::iterator last = bound;

        while (first < last)
        {
            std::vector<Item>::iterator mid =
                first + (last - first) / 2;

            if (mid->value < value.value)
                first = mid + 1;
            else
                last = mid;
        }

        main.insert(first, value);
    }

    return main;
}

// Interface publique : conserve ton vector<int>
std::vector<int> fordJohnson(std::vector<int> arr)
{
    std::vector<Item> items;

    for (size_t i = 0; i < arr.size(); ++i)
        items.push_back(Item(arr[i], i));

    items = fordJohnsonItems(items);

    for (size_t i = 0; i < arr.size(); ++i)
        arr[i] = items[i].value;

    return arr;
}
void fillVec(std::vector<int> &vec, std::deque<int> &deq, char **av, int ac)
{
    for (int i = 1; i < ac; ++i)
    {
        int value = std::atoi(av[i]);
        vec.push_back(value);
        deq.push_back(value);
    }
}

std::deque<Item> fordJohnsonItems(std::deque<Item> arr)
{
    if (arr.size() <= 1)
        return arr;

    // 1. Former les paires
    std::deque<Pair> pairs;

    for (size_t i = 0; i + 1 < arr.size(); i += 2)
    {
        Item a = arr[i];
        Item b = arr[i + 1];

        if (a.value > b.value)
            std::swap(a, b);

        pairs.push_back(Pair(a, b));
    }

    bool hasOdd = (arr.size() % 2 != 0);
    Item odd = hasOdd ? arr.back() : Item();

    // 2. Extraire les maximums
    std::deque<Item> large;

    for (size_t i = 0; i < pairs.size(); ++i)
        large.push_back(pairs[i].large);

    // 3. Récursion : trier uniquement les maximums
    large = fordJohnsonItems(large);

    // 4. Réassocier les petits éléments
    std::deque<Item> small;

    for (size_t i = 0; i < large.size(); ++i)
    {
        for (size_t j = 0; j < pairs.size(); ++j)
        {
            if (pairs[j].large.id == large[i].id)
            {
                small.push_back(pairs[j].small);
                break;
            }
        }
    }

    // 5. Construire la main chain
    std::deque<Item> main;
    main.push_back(small[0]);

    for (size_t i = 0; i < large.size(); ++i)
        main.push_back(large[i]);

    // 6. Insérer les small restants
    // L'élément impair participe aussi à Jacobsthal
    size_t count = small.size() + (hasOdd ? 1 : 0);
    std::deque<size_t> order = jacobsthalOrderD(count);

    for (size_t k = 0; k < order.size(); ++k)
    {
        size_t index = order[k];
        bool isOdd = (index == small.size());

        Item value = isOdd ? odd : small[index];

        // 7. Trouver la borne de recherche
        std::deque<Item>::iterator bound = main.end();

        if (!isOdd)
        {
            for (std::deque<Item>::iterator it = main.begin();
                 it != main.end(); ++it)
            {
                if (it->id == large[index].id)
                {
                    bound = it;
                    break;
                }
            }
        }

        // Recherche dichotomique bornée
        std::deque<Item>::iterator first = main.begin();
        std::deque<Item>::iterator last = bound;

        while (first < last)
        {
            std::deque<Item>::iterator mid =
                first + (last - first) / 2;

            if (mid->value < value.value)
                first = mid + 1;
            else
                last = mid;
        }

        main.insert(first, value);
    }

    return main;
}

// Interface publique : conserve ton deque<int>
std::deque<int> fordJohnson(std::deque<int> arr)
{
    std::deque<Item> items;

    for (size_t i = 0; i < arr.size(); ++i)
        items.push_back(Item(arr[i], i));

    items = fordJohnsonItems(items);

    for (size_t i = 0; i < arr.size(); ++i)
        arr[i] = items[i].value;

    return arr;
}

int main(int ac, char **av)
{
    std::vector<int> arr;
    std::deque<int> deq;
    fillVec(arr, deq, av, ac);

    arr = fordJohnson(arr);
    deq = fordJohnson(deq);

    for (size_t i = 0; i < arr.size(); ++i)
        std::cout << arr[i] << " ";

    std::cout << std::endl;
    std::cout << std::endl;

    for (size_t i = 0; i < deq.size(); ++i)
        std::cout << deq[i] << " ";

    std::cout << std::endl;
    return 0;
}
