#include "StarCategory.h"

#include <string>

// #ff1718
float class_M_base_color[3] = {
    1.0, 0.09, 0.094
};

// #ff8445
float class_K_base_color[3] = {
    1.0, 0.518, 0.271
};

// #ffa261
float class_G_base_color[3] = {
    1.0, 0.635, 0.38
};

// #ffd6aa
float class_F_base_color[3] = {
    1.0, 0.839, 0.667
};

// #ffffff
float class_A_base_color[3] = {
    1.0, 1.0, 1.0
};

// #d8e9fe
float class_B_base_color[3] = {
    0.847, 0.914, 0.996
};

// #629afa
float class_O_base_color[3] = {
    0.384, 0.604, 0.98
};

// #a0a0a0
float white_dwarf_base_color[3] = {
    0.625, 0.625, 0.625
};

// #000040
float black_hole_base_color[3] = {
    0.0, 0.0, 0.25
};

// #402000
float brown_dwarf_base_color[3] = {
    0.25, 0.125, 0.0
};

StarCategory star_categories_[e_NUM_CATEGORIES] = {
    {   "Class M",      sst_MainSequence,   0.8,                class_M_base_color,     0.15,   0.6,    0.06,   0.51    },
    {   "Class K",      sst_MainSequence,   0.88,               class_K_base_color,     0.72,   0.85,   0.67,   0.79    },
    {   "White Dwarf",  sst_None,           0.93,               white_dwarf_base_color, 0.001,  0.01,   0.2,    1.4     },
    {   "Class G",      sst_MainSequence,   0.965,              class_G_base_color,     0.92,   1.1,    0.92,   1.05    },
    {   "Class F",      sst_MainSequence,   0.985,              class_F_base_color,     1.3,    1.5,    1.3,    1.6     },
    {   "Class A",      sst_MainSequence,   0.992,              class_A_base_color,     1.7,    2.4,    2.0,    2.9     },
    {   "Class M",      sst_Giant,          0.9953923252386,    class_M_base_color,     40.0,   80.0,   1.15,   1.25    },
    {   "Brown Dwarf",  sst_None,           0.9983923252386,    brown_dwarf_base_color, 0.06,   0.11,   0.04,   0.06    },
    {   "Class B",      sst_MainSequence,   0.9993923252386,    class_B_base_color,     3.0,    7.4,    3.8,    17.0    },
    {   "Class K",      sst_Giant,          0.99973166776246,   class_K_base_color,     15.0,   25.0,   1.1,    1.2     },
    {   "Class G",      sst_Giant,          0.999880130116649,  class_G_base_color,     6.0,    10.0,   1.0,    1.1     },
    {   "Class F",      sst_Giant,          0.999964965747614,  class_F_base_color,     4.9,    5.1,    1.95,   2.05    },
    {   "Class A",      sst_Giant,          0.999994658218452,  class_A_base_color,     4.9,    5.1,    3.9,    4.1     },
    {   "Class B",      sst_Giant,          0.9999989,          class_B_base_color,     8.0,    15.0,   7.0,    20.0    },
    {   "Class M",      sst_SuperGiant,     0.99999974835622,   class_M_base_color,     490.0,  510.0,  12.5,   13.5    },
    {   "Class O",      sst_MainSequence,   0.999999848356219,  class_O_base_color,     8.5,    15.0,   23.0,   120.0   },
    {   "Class K",      sst_SuperGiant,     0.999999933191841,  class_K_base_color,     200.0,  400.0,  12.5,   13.5    },
    {   "Class G",      sst_SuperGiant,     0.999999970307426,  class_G_base_color,     120.0,  150.0,  10.0,   12.0    },
    {   "Class F",      sst_SuperGiant,     0.999999991516332,  class_F_base_color,     75.0,   85.0,   11.5,   12.5    },
    {   "Class A",      sst_SuperGiant,     0.999999996516332,  class_A_base_color,     55.0,   65.0,   15.5,   16.5    },
    {   "Black Hole",   sst_None,           0.999999998939448,  black_hole_base_color,  0.0001, 0.0005, 24.0,   120.0,  },
    {   "Class B",      sst_SuperGiant,     0.999999999999894,  class_B_base_color,     28.0,   32.0,   22.5,   27.5    },
    {   "Class O",      sst_SuperGiant,     1.0,                class_O_base_color,     28.0,   32.0,   68.0,   72.0    }
};

std::string sequence_type_names_[] = {
    "",
    "Regular",
    "Giant",
    "SuperGiant"
};
