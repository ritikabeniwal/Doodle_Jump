#include "led_matrix_draw_alphabets.h"

//#define PRINT_DEBUG
#define MAX_DIGITS 10
#define ALPHABET_WIDTH 5
#define ALPHABET_HEIGHT 5
#define ALPHABET_COL_MASK 0x1F

#define DIGIT_WIDTH 5
#define DIGIT_HEIGHT 5
#define DIGIT_COL_MASK 0x1F

static const char alphabets[26][ALPHABET_HEIGHT] = {
    {0x1F, 0x11, 0x1F, 0x11, 0X11}, // A
    {0x1F, 0x11, 0x1F, 0x11, 0X1F}, // B
    {0x1F, 0x10, 0x10, 0x10, 0x1F}, // C
    {0x1F, 0x09, 0x09, 0x09, 0X1F}, // D
    {0x1F, 0x10, 0x1E, 0x10, 0x1F}, // E
    {0x1F, 0x10, 0x1F, 0x10, 0x10}, // F
    {0x1F, 0x10, 0x17, 0x15, 0x1D}, // G
    {0x11, 0x11, 0x1F, 0x11, 0x11}, // H
    {0x1F, 0x04, 0x04, 0x04, 0x1F}, // I
    {0x1F, 0x04, 0x04, 0x14, 0x1C}, // J
    {0x11, 0x12, 0x1C, 0x12, 0x11}, // K
    {0x10, 0x10, 0x10, 0x10, 0X1F}, // L
    {0x11, 0x1B, 0x15, 0x11, 0x11}, // M
    {0x11, 0x19, 0x15, 0x13, 0x11}, // N
    {0x1F, 0x11, 0x11, 0x11, 0x1F}, // O
    {0x1F, 0x11, 0x1F, 0x10, 0x10}, // P
    {0x1F, 0x11, 0x18, 0x1F, 0x02}, // Q
    {0x1F, 0x11, 0x1F, 0x14, 0x12}, // R
    {0x1F, 0x10, 0x1F, 0x01, 0x1F}, // S
    {0x1F, 0x04, 0x04, 0x04, 0x04}, // T
    {0x11, 0x11, 0x11, 0x11, 0x1F}, // U
    {0x11, 0x11, 0x11, 0x0A, 0x04}, // V
    {0x11, 0x11, 0x15, 0x1B, 0x11}, // W
    {0x11, 0x0A, 0x04, 0x0A, 0x11}, // X
    {0x11, 0x0A, 0x04, 0x04, 0x04}, // Y
    {0x1F, 0x02, 0x04, 0x08, 0x1F}, // Z
};
static const int digits[10][DIGIT_HEIGHT] = {
    {0x1F, 0x11, 0x11, 0x11, 0X1f}, // 0
    {0x04, 0x0C, 0x14, 0x04, 0X1F}, // 1
    {0x1F, 0x01, 0x1F, 0x10, 0x1F}, // 2
    {0x1F, 0x02, 0x04, 0x02, 0X1F}, // 3
    {0x02, 0x06, 0x10, 0x1F, 0x02}, // 4
    {0x1F, 0x10, 0x1F, 0x01, 0x1F}, // 5
    {0x1F, 0x10, 0x1F, 0x11, 0x1F}, // 6
    {0x1E, 0x02, 0x07, 0x02, 0x02}, // 7
    {0x1F, 0x11, 0x1F, 0x11, 0x1F}, // 8
    {0x1F, 0x11, 0x1F, 0x01, 0x1F}, // 9
};
void led_matrix_print_alphabet(char char_to_display, int row, int col) {}

void led_matrix_draw_alphabets_print_string(char *str, int row, int col, led_matrix__color_e color) {
  char to_display;
  data_size row_data = 0;
  int temp_col = col;
  for (int i = 0; i < ALPHABET_HEIGHT; i++) {
    for (int j = 0; j < strlen(str); j++) {
      to_display = str[j];

#ifdef PRINT_DEBUG
      fprintf(stderr, "temp col = %d\n", temp_col);
      fprintf(stderr, "to_display = %c, %d\n", to_display, to_display - 65);
#endif
      if (to_display == ' ') {
        row_data |= ((((data_size)0)) << (63 - ALPHABET_WIDTH - temp_col));
        temp_col += 1;
      } else {
        row_data |=
            ((((data_size)alphabets[to_display - 65][i] & ALPHABET_COL_MASK)) << (63 - ALPHABET_WIDTH - temp_col));
        temp_col += ALPHABET_WIDTH + 1;
      }
#ifdef PRINT_DEBUG
      fprintf(stderr, "alphabet value = %d row data = %d, temp col = %d\n", (alphabets[to_display - 65][i]), row_data,
              temp_col);
#endif
    }
#ifdef PRINT_DEBUG
    fprintf(stderr, "row data set for row = %d col = %d\n", row + i, col);
#endif
    led_matrix__clear_row_data(row + i);
    led_matrix__set_row_data(row + i, color, row_data);
    row_data = 0;
    temp_col = col;
  }
}

void led_matrix_print_digits_string(int num, int row, int column, led_matrix__color_e color) {
  int rem, arr[MAX_DIGITS], i = 0;
  data_size row_data = 0;
  int temp_col = column;

  if (num == 0) {
    arr[0] = num;
    i++;
  } else {
    while (num != 0) {
      rem = num % 10;
      arr[i] = rem;
      i++;
      num = num / 10;
    }
  }

  for (int j = 0; j < DIGIT_HEIGHT; j++) {
    for (int k = i - 1; k > -1; k--) {
      row_data |= ((((data_size)digits[arr[k]][j] & DIGIT_COL_MASK)) << (63 - DIGIT_WIDTH - temp_col));
      temp_col += DIGIT_WIDTH + 1;
    }
    led_matrix__or_row_data(row + j, color, row_data);
    row_data = 0;
    temp_col = column;
  }
}
