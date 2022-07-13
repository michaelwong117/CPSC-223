valgrind --leak-check=full --show-leak-kinds=all ./PointsetUnit 1
/c/cs223/bin/submit 5 pointset.c log makefile
/c/cs223/bin/testit 5 PointsetUnit

gdb --args ./PointsetUnit 8
time ./GmapUnit 17 10000000 1

// Working pointset create

pointset *pointset_create(const point2d *pts, size_t n)
{
    pointset *result = malloc(sizeof(pointset));

    if (result == NULL) return NULL;

    result->root = NULL;
    result->size = 0;

    if (n == 0 || pts == NULL) return result;

    point2d *sortedpts = malloc(sizeof(point2d) * n);

    for (int i = 0; i < n; i++)
    {
        sortedpts[i] = pts[i];
    }

    // for (int i = 0; i < n; i++)
    // {
    //     printf("pts, %f %f\n", pts[i].x, pts[i].y);
    // }
    
    qsort(sortedpts, n, sizeof(point2d), compare_points);

    // for (int i = 0; i < n; i++)
    // {
    //     printf("sortedpts, %f %f\n", sortedpts[i].x, sortedpts[i].y);
    // }

    median_insert(sortedpts, 0, n-1, result);

    free(sortedpts);

    return result;
}


void median_insert(point2d *sortedpts, int l, int r, pointset *result)
{
    // base case
    if (l > r) return;

    int new_bound = (l + r) / 2;

    point2d *median = &sortedpts[new_bound];

    pointset_add(result, median);

    median_insert(sortedpts, l, new_bound-1, result);
    median_insert(sortedpts, new_bound+1, r, result);
}


for (int i = l; i <= r; i++)
{
    int dir = find_direction(median, sortedpts[i]);
    
    if (dir == 1)
    {
        curr = curr->topleft;
    }
    else if (dir == 2)
    {
        curr = curr->botleft;
    }
    else if (dir == 3)
    {
        curr = curr->topright;
    }
    else if (dir == 4)
    {
        curr = curr->botright;
    }

}


ps_node* median_insert(point2d *quadsorted, int l, int r, pointset *result)
{
    // base case
    if (l > r) return NULL;

    int topleft = 0, botleft = 0, topright = 0, botright = 0;

    point2d *beforequad = malloc(sizeof(point2d) * (r - l + 1));

    int new_bound = (l + r) / 2;

    point2d *median = &quadsorted[new_bound];

    print_point_formatted(median, "Median: %f %f\n");

    for (int i = l; i <= r; i++)
    {
        int dir = find_direction(median, &quadsorted[i]); // printf("direction: %d\n", dir);
        beforequad[i] = quadsorted[i]; // printf("quadsorted, %f %f\n", quadsorted[i].x, quadsorted[i].y);

        if (dir == 1) topleft++;
        else if (dir == 2) botleft++;
        else if (dir == 3) topright++;
        else if (dir == 4) botright++;
    }

    printf("first quad: %d, second quad: %d, third quad: %d, fourth quad: %d\n", 
            topleft, topleft+botleft, topleft+botleft+topright, topleft+botleft+topright+botright);


    // [topleft, botleft, topright, botright]

    int topleftc = 0;
    int botleftc = topleft;
    int toprightc = topleft+botleft;
    int botrightc = topleft+botleft+topright;

    for (int i = l; i <= r; i++)
    {
        int dir = find_direction(median, &quadsorted[i]);

        if (dir == 1)
        {
            printf("beforequad %f %f at idx %d is now at idx %d\n", 
            beforequad[i].x, beforequad[i].y, i, topleftc);
            quadsorted[topleftc] = beforequad[i];
            topleftc++;
        }
        if (dir == 2)
        {
            quadsorted[botleftc] = beforequad[i];
            botleftc++;
        }
        if (dir == 1)
        {
            quadsorted[toprightc] = beforequad[i];
            toprightc++;
        }
        if (dir == 1)
        {
            quadsorted[botrightc] = beforequad[i];
            botrightc++;
        }
    }

    for (int i = l; i <= r; i++)
    {
        printf("quadsorted points: %f %f\n", quadsorted[i].x, quadsorted[i].y);
    }

    ps_node *res = malloc(sizeof(ps_node));

    res->p = median;

    // res->topleft = median_insert(sortedpts, quadsorted, 0, topleft-1, result);
    // res->botleft = median_insert(sortedpts, quadsorted, topleft, topleft+botleft-1, result);
    // res->topright = median_insert(sortedpts, quadsorted, topleft+botleft, topleft+botleft+topright-1, result);
    // res->botright = median_insert(sortedpts, quadsorted, topleft+botleft+topright, topleft+botleft+topright+botright-1, result);

    free(beforequad);

    return res;
}


/**
 * Creates a point set containing copies of the points in the
 * given array of points.  If the array is NULL and n is 0 then
 * the returned tree is empty.  If the tree could not be created
 * then the returned value is NULL.  If the return value is not
 * NULL then it is the caller's responsibility to eventually
 * destroy the point set.
 *
 * @param pts an array of unique valid points, non-NULL if n != 0
 * @param n the number of points in that array, or 0 if pts is NULL
 * @return a pointer to the newly created collection of points, or NULL
 * if it could not be created
 */
pointset *pointset_create(const point2d *pts, size_t n)
{
    pointset *result = malloc(sizeof(pointset));

    if (result == NULL) return NULL;

    result->root = NULL;
    result->size = 0;

    if (n == 0 || pts == NULL) return result;

    point2d *quadsorted = malloc(sizeof(point2d) * n);

    for (int i = 0; i < n; i++)
    {
        quadsorted[i] = pts[i];
    }

    // for (int i = 0; i < n; i++)
    // {
    //     printf("pts, %f %f\n", pts[i].x, pts[i].y);
    // }
    
    qsort(quadsorted, n, sizeof(point2d), compare_points);

    for (int i = 0; i < n; i++)
    {
        printf("quadsorted, %f %f\n", quadsorted[i].x, quadsorted[i].y);
    }

    // at the beginning, the subtree is all nodes
    result->root = median_insert(quadsorted, 0, n-1, result);
    result->size = n;

    free(quadsorted);

    return result;
}
