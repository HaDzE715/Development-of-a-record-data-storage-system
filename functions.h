#include "prog2_ex1.h"

typedef struct track
{
    char *Record_Name;
    char *Track_Name;
    int Track_Length;
    int Track_Position;
}track;

typedef struct Record
{
    char *Record_Name;
    int Record_Year;
    int Record_ID; // Unique key
    int TracksSize;
    int Number_Of_Tracks;
    struct track *Record_Tracks;
    record_category Record_Category;
}Record;

Record *Init_Records(Record *RecordsList);

records_result Add_Record(Record *RecordsArr, char *RecordName, int RecordYear, int TracksNumber, record_category RecordCategory);
records_result Remove_Record(Record *RecordsArr, char *RecordName);
records_result Add_Track_To_Record(Record *RecordsArr, char *RecordName, char *TrackName, int TrackLength, int TrackPosition);
records_result Report_Records(Record *RecordsArr, FILE *out, record_category RecordCategory);
records_result Report_Tracks(Record *RecordsArr, FILE *out, char *RecordName);
records_result Report_Containing_Records(Record *RecordsArr, FILE *out, char *TrackName);

int Compare_Record_Name(const void *a, const void *b);
int Execute_Records_DB(int argc,char *input_file_name, char *output_file_name);

void Report_Errors(records_result res);
void Sort_Records_Lexicographically(Record *RecList);
void Finalize_Records(Record *RecList);