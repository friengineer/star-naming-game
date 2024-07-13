#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

typedef struct {
  int ID;
  double x;
  double y;
  char name[100];
} Star;
typedef struct {
  int ID;
  int ID1;
  int ID2;
  double dist;
} Pair;
Star stars[1800];
Pair pairs[900];
char frame_buffer[30][60];

// puts the designated character into the specified location in frame_buffer
void Plot(int x, int y, char c) {
  frame_buffer[y][x] = c;
}

// sets all elements of frame_buffer to ' '
void Clear() {
  for (int i = 0; i < 30; i++) {
    for (int k = 0; k < 60; k++) {
      frame_buffer[i][k] = ' ';
    }
  }
}

// writes the names of paired stars underneath or above them
void WriteAt(int x, int y, char str[]) {
  printf("%s\n", str);

  int i = strlen(str);

  for (int k = 0; k < i; k++) {
    frame_buffer[y][x + k] = str[k];
  }
}

// prints out frame_buffer in a 60x30 grid
void Refresh() {
  system("clear");

  for (int i = 0; i < 30; i++) {
    for (int k = 0; k < 60; k++) {
      printf("%c", frame_buffer[i][k]);
    }

    printf("\n");
  }
}

// initialises stars array
void InitialiseStars() {
  for (int i = 0; i < 1800; i++) {
    stars[i].ID = -1;
    stars[i].x = -1;
    stars[i].y = -1;
    strcpy(stars[i].name, "");
  }
}

// initialises pairs array
void InitialisePairs() {
  for (int i = 0; i < 900; i++) {
    pairs[i].ID = -1;
    pairs[i].ID1 = -1;
    pairs[i].ID2 = -1;
    pairs[i].dist = -1;
  }
}

void main() {
  char com[4];
  int num = 0;
  bool quit = false;

  InitialiseStars();
  InitialisePairs();

  Clear();

  time_t t;
  // seeds random generator using the time so that a different sequence of random numbers will be generated every time
  srand((unsigned) time(&t));

  // keeps displaying the menu until 'quit' has been entered
  while (quit == false) {
    printf("Enter a command:\n");
    printf(">>>");
    scanf("%s", com);

    // generates and plots the specified number of random stars in frame_buffer
    if (strcmp(com, "bang") == 0) {
      InitialiseStars();
      InitialisePairs();
      Clear();

      scanf("%i", &num);

      // checks that a valid positive number of stars has been entered
      if (num > 1800) {
        printf("\nThe maximum number of stars you can create is 1800 so 1800 stars will be created instead.\n");
        num = 1800;
      }

      // checks if a negative number of stars has been entered
      if (num < 0) {
        printf("\nYou cannot create a negative number of stars. 0 stars will be created instead.\n");
        num = 0;
      }

      int ID = 0;

      // generates the random coordinates for the specified number of stars ensuring that no two generated coordinates are the same
      for (int i = 1; i <= num; i++) {
        int x = rand() % 60;
        int y = rand() % 30;

        if (frame_buffer[y][x] == '.') {
          i--;
          continue;
        }
        else {
          stars[ID].ID = ID;
          stars[ID].x = x;
          stars[ID].y = y;
          strcpy(stars[ID].name, "");
          ID++;
          Plot((int)x, (int)y, '.');
        }
      }

      printf("\nThe Big Bang has just occurred!\n\n");
    }
    // lists all of the stars in the stars array
    else if (strcmp(com, "list") == 0) {
      if (stars[0].ID == -1) {
        printf("\nThere are no stars.\n");
      }
      else {
        for (int i = 0; i < 1800; i++) {
          // print out the information for each star. Stops reading star data if a star has not been generated
          if (stars[i].ID == -1) {
            printf("\n");
            break;
          }
          else {
            printf("\n");
            printf("Star %i\n", stars[i].ID);

            if (strcmp(stars[i].name, "") != 0) {
              printf("Name: %s\n", stars[i].name);
            }

            printf("Coordinate: (%f,%f)", stars[i].x, stars[i].y);
            printf("\n");
          }
        }
      }
      printf("\n");
    }
    // name the two stars that are closest to each other
    else if (strcmp(com, "name") == 0) {
      int count = 0;

      Star avail_stars[1800];

      // initialises avail_stars array
      for (int i = 0; i < 1800; i++) {
        avail_stars[i].ID = -1;
        avail_stars[i].x = -1;
        avail_stars[i].y = -1;
        strcpy(avail_stars[i].name, "");
      }

      printf("\n");

      // puts unnamed stars into avail_stars array
      for (int i = 0; i < 1800; i++) {
        if (strcmp(stars[i].name, "") == 0) {
          avail_stars[count].ID = stars[i].ID;
          avail_stars[count].x = stars[i].x;
          avail_stars[count].y = stars[i].y;
          count++;
        }
      }

      count = 0;

      // counts number of unnamed stars
      for (int i = 0; i < 1800; i++) {
        if (avail_stars[i].ID != -1) {
          count++;
        }
        else {
          break;
        }
      }

      // finds the closest pair of stars provided there are enough unnamed stars to be named and assigns them names if the user
      // would like to
      if (count < 2) {
        printf("Sorry, there are no more pairs available.\n");
      }
      else {
        double closest = 66;
        int star1;
        int star2;
        double dist;
        char input;
        char first[100];
        char last[100];

        for (int i = 0; i < 1799; i++) {
          if (avail_stars[i].ID == -1) {
            break;
          }

          for (int k = (i + 1); k < 1800; k++) {
            if (avail_stars[k].ID == -1) {
              break;
            }

            // calculates the distance between two stars
            dist = sqrt((pow(avail_stars[i].x - avail_stars[k].x, 2)) + ((pow(avail_stars[i].y - avail_stars[k].y, 2))));

            // adjusts the values of closest, star1 and star2 if a pair of closer stars has been found
            if (dist < closest) {
              closest = dist;
              star1 = avail_stars[i].ID;
              star2 = avail_stars[k].ID;
            }
          }
        }

        // diplays the details of the closest pair of stars
        printf("The closest pair of stars is stars %i and %i\n", star1, star2);
        printf("They are %f light years apart\n", closest);
        printf("Would you like to name this pair? (y/n): ");
        scanf(" %c", &input);

        // asks the user if they would like to name the stars and if so collects the names from the user
        if (input != 'n') {
          if(input == 'y') {
            getchar();
            printf("Enter your full name (100 characters maximum): ");
            scanf("%[^\n]%*c", first);
            printf("Enter your spouse's full name(100 characters maximum): ");
            scanf("%[^\n]%*c", last);

            int position = 0;

            // finds the next empty space in the named pairs array
            for (int i = 0; i < 900; i++) {
              if (pairs[i].ID == -1) {
                break;
              }

              position++;
            }

            // stores named star pair information
            pairs[position].ID = position;
            pairs[position].ID1 = star1;
            pairs[position].ID2 = star2;
            pairs[position].dist = closest;
            strcpy(stars[star1].name, first);
            strcpy(stars[star2].name, last);

            // updates frame_buffer with new named star pair
            Plot((int) stars[star1].x, (int) stars[star1].y, '*');
            Plot((int) stars[star2].x, (int) stars[star2].y, '*');

            printf("Congratulations! A pair of stars has been named after you and your spouse.\n");
          }
          else {
            printf("Enter either 'y' or 'n' for yes or no.\n");
            getchar();
          }
        }
      }

      printf("\n\n");
    }
    // displays the information of all the named pairs of stars
    else if (strcmp(com, "pairs") == 0) {
      if (pairs[0].ID == -1) {
        printf("\nThere are no pairs of named stars.\n");
      }
      else {
        for (int i = 0; i < 900; i++) {
          if (pairs[i].ID == -1) {
            printf("\n");
            break;
          }
          else {
            printf("\n");
            printf("Pair %i:\n", pairs[i].ID);
            printf("  Star %i:\n", pairs[i].ID1);
            printf("    Name: %s\n", stars[pairs[i].ID1].name);
            printf("    Coordinates: (%f,%f)\n", stars[pairs[i].ID1].x, stars[pairs[i].ID1].y);
            printf("  Star %i:\n", pairs[i].ID2);
            printf("    Name: %s\n", stars[pairs[i].ID2].name);
            printf("    Coordinates: (%f,%f)\n", stars[pairs[i].ID2].x, stars[pairs[i].ID2].y);
            printf("  Distance between stars: %f light years\n", pairs[i].dist);
          }
        }
      }

      printf("\n");
    }
    // displays all the stars
    else if (strcmp(com, "draw") == 0) {
      Refresh();
      printf("\n");
    }
    // display information about a star with the supplied name
    else if (strcmp(com, "show") == 0) {
      char name[100];
      int star = -1;

      getchar();
      printf("Enter your full name (100 characters maximum): ");
      scanf("%[^\n]%*c", name);

      // looks for star with supplied name
      for (int i = 0; i < 1800; i++) {
        if (strcmp(name, stars[i].name) == 0) {
          star = stars[i].ID;
          break;
        }
      }

      if (star == -1) {
        printf("\nYou do not have a star named after you.\n");
      }
      else {
        char backup_buffer[30][60];

        for (int i = 0; i < 130; i++) {
          for (int k = 0; k < 60; k++) {
            backup_buffer[i][k] = frame_buffer[i][k];
          }
        }

        for (int i = 0; i < 900; i++) {
          // finds the star the named star is paired with
          if (pairs[i].ID1 == star || pairs[i].ID2 == star) {
            char string[60];
            strcpy(string, stars[pairs[i].ID1].name);
            strcat(string, " & ");
            strcat(string, stars[pairs[i].ID2].name);

            int x;

            // determines where to write star names depending on if star is at an edge of the display
            if (stars[pairs[i].ID1].y >= stars[pairs[i].ID2].y) {
              if (stars[pairs[i].ID1].y == 29) {
                WriteAt((int) stars[pairs[i].ID1].x, (int) stars[pairs[i].ID1].y - 1, string);
                Refresh();
              }
              else {
                WriteAt((int) stars[pairs[i].ID1].x, (int) stars[pairs[i].ID1].y + 1, string);
                Refresh();
              }
            }
            else {
              if (stars[pairs[i].ID2].y == 29) {
                WriteAt((int) stars[pairs[i].ID2].x, (int) stars[pairs[i].ID2].y - 1, string);
                Refresh();
              }
              else {
                WriteAt((int) stars[pairs[i].ID2].x, (int) stars[pairs[i].ID2].y + 1, string);
                Refresh();
              }
            }
          }
        }

        // restores frame_buffer to its original state
        for (int i = 0; i < 130; i++) {
          for (int k = 0; k < 60; k++) {
            frame_buffer[i][k] = backup_buffer[i][k];
          }
        }
      }

      printf("\n");
    }
    // saves the stars and pairs to a binary file
    else if (strcmp(com, "save") == 0) {
      FILE *writer = fopen("universe.bin", "wb");

      for (int i = 0; i < 1800; i++) {
       int ID = stars[i].ID;
       double x = stars[i].x;
       double y = stars[i].y;

       fwrite(&ID, sizeof(int), 1, writer);
       fwrite(&x, sizeof(double), 1, writer);
       fwrite(&y, sizeof(double), 1, writer);
       fwrite(stars[i].name, sizeof(char), 100, writer);
      }

      fwrite(pairs, sizeof(Pair), 900, writer);
      fclose(writer);

      printf("\nYour universe has been saved.\n\n", com);
    }
    // loads stars and pairs from a binary file
    else if (strcmp(com, "load") == 0) {
      FILE *reader = fopen("universe.bin", "rb");

      for (int i = 0; i < 1800; i++) {
        fread(&stars[i].ID, sizeof(int), 1, reader);
        fread(&stars[i].x, sizeof(double), 1, reader);
        fread(&stars[i].y, sizeof(double), 1, reader);
        fread(stars[i].name, sizeof(char), 100, reader);
      }

      fread(pairs, sizeof(Pair), 900, reader);
      fclose(reader);

      Clear();

      for (int i = 0; i < 900; i++) {
        if (pairs[i].ID != -1) {
          Plot(stars[pairs[i].ID1].x, stars[pairs[i].ID1].y, '*');
          Plot(stars[pairs[i].ID2].x, stars[pairs[i].ID2].y, '*');
        }
      }

      for (int i = 0; i < 1800; i++) {
        if (frame_buffer[(int) stars[i].y][(int) stars[i].x] == ' ') {
          Plot(stars[i].x, stars[i].y, '.');
        }
      }

      printf("\nYour universe has been restored.\n\n");
    }
    // quits the program
    else if (strcmp(com, "quit") == 0) {
      break;
    }
    else {
      printf("\nEnter a valid command.\n\n");
    }
  }

  printf("\nYou have quit the program.\n");
  exit(0);
}
