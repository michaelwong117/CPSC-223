
  if (argc > 1) // given case
  {

    if(strcmp(argv[2], "-given") == 0) // if the two strings are equal, then -given case
    {

    }
    else{
      fprintf(stderr,"TSP: invalid algorithm arguments\n");
      exit(-1);
    }
  }
  if (argc > 2) // farthest case
  {
    if(strcmp(argv[3], "-farthest") == 0)
    {
      
    }
    else{
      fprintf(stderr,"TSP: invalid algorithm arguments\n");
      exit(-1);
    }
  }
  if (argc > 3) // exchange
  {
    if (strcmp(argv[4], "-exchange") == 0)
    {
      if (strcmp(argv[5], "adjacent") == 0) // adjacent case
      {

      }
      else if (strcmp(argv[5], "any") == 0) // any case
      {

      }
      else{
        fprintf(stderr,"TSP: invalid algorithm arguments\n");
        exit(-1);
      }
    }
    else{
      fprintf(stderr,"TSP: invalid algorithm arguments\n");
      exit(-1);
    }
  }
  if (argc > 5) // exchange
  {
    if (strcmp(argv[6], "-exchange") == 0)
    {
      if (strcmp(argv[7], "adjacent") == 0) // adjacent case
      {

      }
      else if (strcmp(argv[7], "any") == 0) // any case
      {

      }
      else{
        fprintf(stderr,"TSP: invalid algorithm arguments\n");
        exit(-1);
      }
    }
    else{
      fprintf(stderr,"TSP: invalid algorithm arguments\n");
      exit(-1);
    }
  }
  if (argc > 8) // too many arguments, this means more than 8 arguments total
  {
    fprintf(stderr,"TSP: invalid algorithm arguments\n");
    exit(-1);
  }

   temp[i] = Adjacent[i];
  }
  printf("\n");
  printf("temp: ");
  for (int i = 0; i < n; i++)
  {
    printf("%s ", temp[i].name);
  }
  printf("\n");

  for (int i = start; i < n; i++)
  {
    Adjacent[i - start] = Adjacent[i];
  }
  // n - start is the amount of space the temp block takes up

  int j = 0;

  for (int i = n - start; i < n; i++)
  {
    Adjacent[i] = temp[j];
    j++;
  }


  printf("from start: ");
  for (int i = start; i < n; i++)
  {
    printf("%s ", Adjacent[i].name);
    Adjacent[i-start] = Adjacent[i];
  }
  printf("\n");
  // printf("start: %d ", start);
  for (int i = 0; i < start; i++)
  {
    Adjacent[i] = temp[i];
  }
  printf("Final order: ");
  for (int i = 0; i < n; i++)
  {
    printf("%s ", Adjacent[i].name);
  }
  printf("\n");



 double sum = 0;
  for (int i = 0; i < n - 1; i++)
  {
    sum += location_distance(&Adjacent[i].L, &Adjacent[i+1].L);
  }
  sum += location_distance(&Adjacent[n-1].L, &Adjacent[0].L);
  



  while (nolower != -1)
  {
    double max = -1; int swap1, swap2;

    for (int i = 0; i < n - 1; i++)
    {
      City temp = Adjacent[i+1];
      Adjacent[i+1] = Adjacent[i];
      Adjacent[i] = temp;

      double curr = 0;

      for (int i = 0; i < n - 1; i++)
      {
        curr += location_distance(&Adjacent[i].L, &Adjacent[i+1].L);
      }
      curr += location_distance(&Adjacent[n-1].L, &Adjacent[0].L);

      if (curr < sum)
      {
        if (max < sum - curr) // choose the greatest decrease in difference
        {
          max = sum - curr;
          swap1 = i;
          swap2 = i+1; // store the indexes to actually swap them in the array later
        }
      }
    }
    
  }