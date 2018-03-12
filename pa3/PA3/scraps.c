
/*
  UNUSED(argc);

  FILE * f;
  const char * filename = argv[1];

  char read[MAX_NAME_LENGTH];

  f = fopen(filename, "r");
  int i = 0;
  int j = 0;
  int k = 0;
  size_t l = 0;
  do {
    do {
      l = fread(&read[i], 1, 1, f);
      if(l == 0) {
        j = 1;
      }
      i++;
    } while(read[i - 1] != 0x0a);
    i = 0;
//    j++;
    printf("Read: ");
    do {
      printf("%c", read[k]);
      k++;
    } while(read[k - 1] != 0x0a);
    k = 0;
  } while(j == 0);
*/
/*
  UNUSED(argc);
  UNUSED(argv);

  lifo * list = make_lifo(20);

  int ret;
  char output[MAX_NAME_LENGTH];
  int length = 0;

  ret = push(list, "google.com");
  printf("push returned %d\n", ret);

  length = pop(list, output);
  printf("Output string: ");
  for(ret = 0; ret < length; ret++) {
    printf("%c", output[ret]);
  }
  printf("\n");

  return 0;
*/
/*
  if(argc < 2) {
    printf("Need hostname");
    return -1;
  }
  const char* hostname = argv[1];
  char ip[MAX_IP_LENGTH];

  int ret = dnslookup(hostname, ip, MAX_IP_LENGTH);
  if(ret == 0) {
    printf("Success\n");
    printf("%s\n", ip);
  }
  else {
    printf("Failure\n");
  }
  return 0;
*/
