#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#include "pointset.h"
#include "point2d.h"
#include "pqueue.h"

char *ps_error = "error";

/**
 * Pointset Nodes
 */
typedef struct _ps_node
{
    point2d *p;
    struct _ps_node *parent;
    struct _ps_node *topleft;
    struct _ps_node *botleft;
    struct _ps_node *topright;
    struct _ps_node *botright;

} ps_node;
// what you're inserting into the queue
typedef struct _queue_obj
{
    ps_node *node;
    point2d p;
    bool isnode; // 0 for point, 1 for node

    double x_leftbound;
    double x_rightbound;
    double y_lowerbound;
    double y_upperbound;

} queue_obj;

/**
 * Meta-data struct for a Pointset; a set of points implemented using a quadtree
 */
struct _pointset
{
    ps_node *root;
    size_t size;
};


int compare_points(const void *a, const void *b)
{
    point2d *p1 = (point2d *)(a);
    point2d *p2 = (point2d *)(b);

    double p1x = p1->x;
    double p2x = p2->x;
    double p1y = p1->y;
    double p2y = p2->y;

    if (p1x < p2x)
    {
        return -1;
    }
    else if (p1x > p2x)
    {
        return 1;
    }
    else if (p1x == p2x)
    {
        if (p1y <= p2y)
        {
            return -1;
        }
        else if (p1y > p2y)
        {
            return 1;
        }
    }
}
ps_node* median_insert(point2d *quad, int l, int r, pointset *result)
{
    // base case
    if (l > r) return NULL;

    int topleft = 0, botleft = 0, topright = 0, botright = 0;

    int new_bound = (l + r) / 2;

    point2d *median = malloc(sizeof(point2d));
    *median = quad[new_bound];   

    // printf("New call to median_insert\n\n");

    // for (int i = l; i <= r; i++)
    // {
    //     printf("quad %f %f\n", quad[i].x, quad[i].y);
    // }
    // print_point_formatted(median, "Median: %f %f\n");

    for (int i = l; i <= r; i++)
    {
        int dir = find_direction(median, &quad[i]); // printf("direction: %d\n", dir);
        if (dir == 1) topleft++;
        else if (dir == 2) botleft++;
        else if (dir == 3) topright++;
        else if (dir == 4) botright++;
    } 

    point2d *tleft = malloc(sizeof(point2d) * topleft);
    point2d *bleft = malloc(sizeof(point2d) * botleft);
    point2d *tright = malloc(sizeof(point2d) * topright);
    point2d *bright = malloc(sizeof(point2d) * botright);

    // printf("first quad: %d, second quad: %d, third quad: %d, fourth quad: %d\n", 
    //     topleft, botleft, topright, botright);


    int topleftc = 0, botleftc = 0, toprightc = 0, botrightc = 0;

    for (int i = l; i <= r; i++)
    {
        int dir = find_direction(median, &quad[i]);

        // printf("quad %f %f\n", quad[i].x, quad[i].y);

        if (dir == 1)
        {
            tleft[topleftc] = quad[i];
            // printf("topleft %f %f\n", tleft[topleftc].x, tleft[topleftc].y);
            topleftc++;
        }
        if (dir == 2)
        {
            bleft[botleftc] = quad[i];
            // printf("botleft %f %f\n", bleft[botleftc].x, bleft[botleftc].y);
            botleftc++;
        }
        if (dir == 3)
        {
            tright[toprightc] = quad[i];
            // printf("topright %f %f\n", tright[toprightc].x, tright[toprightc].y);
            toprightc++;
        }
        if (dir == 4)
        {
            bright[botrightc] = quad[i];
            // printf("botright %f %f\n", bright[botrightc].x, bright[botrightc].y);
            botrightc++;
        }
    }

    ps_node *res = malloc(sizeof(ps_node));

    res->p = median;

    res->topleft = median_insert(tleft, 0, topleft-1, result);
    res->botleft = median_insert(bleft, 0, botleft-1, result);
    res->topright = median_insert(tright, 0, topright-1, result);
    res->botright = median_insert(bright, 0, botright-1, result);

    free(tleft);
    free(bleft);
    free(tright);
    free(bright);

    return res;
}

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

    result->root = median_insert(sortedpts, 0, n-1, result);
    result->size = n;

    free(sortedpts);

    return result;
}

size_t pointset_size(const pointset *t)
{
    if (t != NULL)
    {
        return t->size;
    }
    else
    {
        return 0;
    }
}
// 0 is the same, topleft is 1, botleft is 2, topright is 3, botright is 4
int find_direction(const point2d *center, const point2d *new)
{
    if (new->x == center->x && new->y == center->y) return 0;
    // printf("center: %f %f, new: %f %f\n", center->x, center->y, new->x, new->y);
    if (new->x <= center->x)
    {
        if (new->y >= center->y)
        {
            return 1;
        }
        else if (new->y < center->y)
        {
            return 2;
        }
    }
    else if (new->x > center->x)
    {
        if (new->y >= center->y)
        {
            return 3;
        }
        else if (new->y < center->y)
        {
            return 4;
        } 
    }
}

bool pointset_add(pointset *t, const point2d *pt)
{
    // take the points, you make a node, then you check if there is a node for this pointset
    // if there is a root, then you iterate along the tree until you find it

    if (t == NULL || pt == NULL) return 0;

    ps_node *new_node = malloc(sizeof(ps_node));
    point2d *copy = malloc(sizeof(point2d));

    if (new_node == NULL || copy == NULL)
    {

        if (new_node != NULL) free(new_node);
        if (copy != NULL) free(copy);
        return 0;
    }

    *copy = *pt;

    new_node->p = copy;

    new_node->topleft = NULL;
    new_node->botleft = NULL;
    new_node->topright = NULL;
    new_node->botright = NULL;

    if (t->root == NULL)
    {
        t->root = new_node;
        t->size++;
        // print_point_formatted(new_node->p, "Added root: %f %f\n");
        return 1;
    }
    ps_node *parent = NULL;
    ps_node *curr = t->root; // center
    int dir;

    while (curr != NULL)
    {
        parent = curr;
        dir = find_direction(curr->p, new_node->p);
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
        else if (dir == 0) // duplicate case
        {
            free(new_node);
            free(copy);
            return 0; // already in tree, return 0;
        }
    }
    // print_point_formatted(new_node->p, "Added: %f %f\n");
    if (dir == 1)
    {
        parent->topleft = new_node;
    }
    else if (dir == 2)
    {
        parent->botleft = new_node;
    }
    else if (dir == 3)
    {
        parent->topright = new_node;
    }
    else if (dir == 4)
    {
        parent->botright = new_node;
    }
    t->size++;
    return 1;

}

bool pointset_contains(const pointset *t, const point2d *pt)
{

    if (t == NULL || pt == NULL) return 0;

    if (t->root == NULL)
    {
        return 0;
    }
    
    ps_node *parent = NULL;
    ps_node *curr = t->root; // center
    int dir;

    while (curr != NULL)
    {
        parent = curr;
        dir = find_direction(curr->p, pt);
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
        else if (dir == 0) // duplicate case
        {
            return 1; // already in tree, found node!
        }
    }
    
    return 0;
}

void pointset_nearest_neighbor(const pointset *t, const point2d *pt, point2d *neighbor, double *d)
{
    if (t == NULL || pt == NULL)
    {
        *d = INFINITY;
        return;
    }

    point2d *nearest = pointset_k_nearest(t, pt, 1);
    
    if (nearest == NULL)
    {
        *d = INFINITY;
        return; 
    }

    neighbor->x = nearest[0].x;
    neighbor->y = nearest[0].y;

    // *neighbor = nearest[0];

    *d = point2d_distance(pt, neighbor);

    free(nearest);

    return;

}

// creates either node or point objects for queuing 
queue_obj *create_child_node(const queue_obj *parent, ps_node *curr)
{
    if (curr == NULL) return NULL;
    queue_obj *child = malloc(sizeof(queue_obj));

    if (child == NULL) return NULL;

    child->node = curr;
    child->isnode = 1;
    child->p = *(child->node->p);
    // child->p = NULL;

    child->x_leftbound = parent->x_leftbound;
    child->x_rightbound = parent->x_rightbound;
    child->y_lowerbound = parent->y_lowerbound;
    child->y_upperbound = parent->y_upperbound;

    return child;

}

/**
 * Returns the k nearest neighbors to the given point among points in the
 * given point set.  Ties are broken arbitrarily.  The k nearest neighbors
 * are returned in array in increasing order of distance.  If the set contains
 * fewer than k points then only the elements in the returned array up
 * to the size of the set will be valid.  If there is an allocation error
 * during the operation, then the return value is NULL.  It is the caller's
 * responsibility to free the returned array.
 *
 * @param t a pointer to a valid pointset, non-NULL
 * @param pt a pointer to a valid point with finite coordinates, non-NULL
 * @param k a positive integer
 */
point2d *pointset_k_nearest(const pointset *t, const point2d *pt, size_t k)
{
/** pointset_k_nearest(points, query, k)
     * q = pqueue_create()
     * point2d nearest[k]
     * found = 0
     * 
     * pqueue_enqueue(q, point->root, 0)
     * while (q is not empty and found < k)
     *  item = pqueue_dequeue(q)
     * if item is a point
        * add item to nearest
        * found++
    * else (item must be a node in this case)
        * pqueue_enqueue(pt inside node, distance from query to that point)
        * for each non-NULL child of node
            * pqueue_enqueue(child, distance from query to region of child)
*/ 
    if (t == NULL || pt == NULL || k == 0) return NULL;
    if (t->root == NULL) return NULL;
    
    pqueue *q = pqueue_create();
    point2d *nearest = malloc(sizeof(point2d) * k);
    size_t found = 0;

    queue_obj *root = malloc(sizeof(queue_obj));

    root->node = t->root;
    root->isnode = 1;
    // print_point_formatted(t->root->p, "t->root->p: %f %f\n");
    root->p = *(t->root->p); // not used for node, helps w debugging
    // print_point_formatted(&root->p, "root->p: %f %f\n");
    // printf("root->p without address: %f %f\n", root->p.x, root->p.y);
    // root->p = NULL;

    root->x_leftbound = -INFINITY;
    root->x_rightbound = INFINITY;
    root->y_lowerbound = -INFINITY;
    root->y_upperbound = INFINITY;


    pqueue_enqueue(q, 0, root); // adds an item to priority queue with given priority
    while (pqueue_size(q) != 0 && found < k)
    {
        // anything you dequeue, is no longer in the queue, and 
        // thus will not be freed with pqueue_destroy
        queue_obj *item = pqueue_dequeue(q, NULL); // returns item with lowest numeric priority
        if (item->isnode == 0) 
        {
            nearest[found] = item->p;
            found++;
        }
        else if (item->isnode == 1) // ITEM IS THE PARENT
        {
            // create the point object to insert into the queue
            queue_obj *curr_point = malloc(sizeof(queue_obj));
            
            curr_point->node = NULL;
            curr_point->isnode = 0;
            curr_point->p = *(item->node->p);

            pqueue_enqueue(q, point2d_distance(pt, &curr_point->p), curr_point);

            // now create the non-NULL child nodes

            queue_obj *topleft = create_child_node(item, item->node->topleft); // top left

            if (topleft != NULL)
            {
                topleft->x_rightbound = item->p.x;
                topleft->y_lowerbound = item->p.y;

                point2d lower_left = {topleft->x_leftbound, topleft->y_lowerbound};
                point2d upper_right = {topleft->x_rightbound, topleft->y_upperbound};

                pqueue_enqueue(q, point2d_distance_to_rectangle(pt, &lower_left, &upper_right), topleft);
            
            }
            
            queue_obj *botleft = create_child_node(item, item->node->botleft); // bot left

            if (botleft != NULL)
            {
                botleft->x_rightbound = item->p.x;
                botleft->y_upperbound = item->p.y;

                point2d lower_left = {botleft->x_leftbound, botleft->y_lowerbound};
                point2d upper_right = {botleft->x_rightbound, botleft->y_upperbound};

                pqueue_enqueue(q, point2d_distance_to_rectangle(pt, &lower_left, &upper_right), botleft);
            
            }
            
            queue_obj *topright = create_child_node(item, item->node->topright); // top right

            if (topright != NULL)
            {
                topright->x_leftbound = item->p.x;
                topright->y_lowerbound = item->p.y;

                point2d lower_left = {topright->x_leftbound, topright->y_lowerbound};
                point2d upper_right = {topright->x_rightbound, topright->y_upperbound};

                pqueue_enqueue(q, point2d_distance_to_rectangle(pt, &lower_left, &upper_right), topright);
            
            }

            queue_obj *botright = create_child_node(item, item->node->botright); // bot right

            if (botright != NULL)
            {
                botright->x_leftbound = item->p.x;
                botright->y_upperbound = item->p.y;

                point2d lower_left = {botright->x_leftbound, botright->y_lowerbound};
                point2d upper_right = {botright->x_rightbound, botright->y_upperbound};

                pqueue_enqueue(q, point2d_distance_to_rectangle(pt, &lower_left, &upper_right), botright);
            
            }

        }
        free(item);
    }
    pqueue_destroy(q, free);
    return nearest;
}

void pointset_for_each(const pointset* t, void (*f)(const point2d *, void *), void *arg)
{
    if (t == NULL || f == NULL) return;
    
    ps_node **queue = malloc(sizeof(ps_node *) * t->size);

    int topop = 0;
    int topush = 1;

    if (t->root == NULL)
    {
        free(queue);
        return; 
    }

    queue[topop] = t->root;

    while(topop < t->size)
    {
        ps_node *curr = queue[topop];
        topop++; 

        // print_point_formatted(curr->p, "Queuing for destroy: %f %f\n");

        if (curr->topleft != NULL)
        {
            queue[topush] = curr->topleft;
            topush++;
        }
        if (curr->botleft != NULL)
        {
            queue[topush] = curr->botleft;
            topush++;
        }
        if (curr->topright != NULL)
        {
            queue[topush] = curr->topright;
            topush++;
        }
        if (curr->botright != NULL)
        {
            queue[topush] = curr->botright;
            topush++;
        }

    }
    for (int i = 0; i < t->size; i++)
    {
        if(queue[i] != NULL)
        // print_point_formatted(queue[i]->p, "For each: %f %f\n");
        f(queue[i]->p, arg);
    }
    free(queue);
}

void pointset_destroy(pointset *t)
{
    if (t == NULL) return;

    ps_node **queue = malloc(sizeof(ps_node *) * t->size);

    int topop = 0;
    int topush = 1;

    if (t->root == NULL)
    {
        free(t);
        free(queue);
        return; 
    }

    queue[topop] = t->root;

    while(topop < t->size)
    {
        ps_node *curr = queue[topop];
        topop++; 

        // print_point_formatted(curr->p, "Queuing for destroy: %f %f\n");

        if (curr->topleft != NULL)
        {
            queue[topush] = curr->topleft;
            topush++;
        }
        if (curr->botleft != NULL)
        {
            queue[topush] = curr->botleft;
            topush++;
        }
        if (curr->topright != NULL)
        {
            queue[topush] = curr->topright;
            topush++;
        }
        if (curr->botright != NULL)
        {
            queue[topush] = curr->botright;
            topush++;
        }

    }
    for (int i = 0; i < t->size; i++)
    {
        if(queue[i] != NULL)
        // print_point_formatted(queue[i]->p, "Destroyed: %f %f\n");
        free(queue[i]->p);
        free(queue[i]);
    }
    free(queue);
    free(t);

}

