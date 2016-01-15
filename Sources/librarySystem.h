#pragma once

// DLL export macro definition.
#ifdef ISIMIDSAGITTAL_EXPORT_SYMBOLS
  // Replace with export symbol if compiling project.
  #define ISIMIDSAGITTAL_EXPORT ML_LIBRARY_EXPORT_ATTRIBUTE
#else
  // Replace with import symbols if included by external sources.
  #define ISIMIDSAGITTAL_EXPORT ML_LIBRARY_IMPORT_ATTRIBUTE
#endif