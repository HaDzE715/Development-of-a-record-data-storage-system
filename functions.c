#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "prog2_ex1.h"

#define RECSIZE 10
#define MAX_LINE_LENGTH 100


Record *Init_Records(Record *RecordsList) // initialize the array of RecordsList
{   
    RecordsList = (Record*)malloc(sizeof(Record) * RECSIZE);

    if(RecordsList == NULL) // Checks if memory allocation succeeded!
        return NULL;


    for(int i = 0; i < RECSIZE; ++i)
    {
        RecordsList[i].Record_Name = NULL;
        RecordsList[i].Record_Category = -1;
        RecordsList[i].Record_Year = 0;
        RecordsList[i].Record_ID = 0;
        RecordsList[i].TracksSize = 0;
        RecordsList[i].Number_Of_Tracks = 0;
    }

    return RecordsList;
}

records_result Add_Record(Record *RecordsArr, char *RecordName, int RecordYear, int TracksNumber, record_category RecordCategory)
{    
    for(int i = 0; i < RECSIZE; ++i) // CHECKS IF RECORD ALREADY EXISTS
        if(RecordsArr[i].Record_ID != 0)
            if(strcmp(RecordsArr[i].Record_Name, RecordName) == 0)
                return RECORD_ALREADY_EXISTS;
    
    if(RecordYear < 1900)
        return INVALID_YEAR;
    if(TracksNumber < 0)
        return INVALID_NUM_OF_TRACKS;
    if(RecordCategory > 6 || RecordCategory < 0)
        return INVALID_CATEGORY;

    /*INITALIZING TRACKS STRUCT*/
    for(int i = 0; i < RECSIZE; ++i)
        RecordsArr[i].Record_Tracks = (track*)malloc(sizeof(track) * TracksNumber);

    for(int i = 0; i < RECSIZE; ++i)
        if(RecordsArr[i].Record_Tracks == NULL)
            return OUT_OF_MEMORY;

    for(int i = 0; i < RECSIZE; ++i)
    {
        for(int k = 0; k < TracksNumber; ++k) // INITALIZING RECORD TRACKS ARRAY
        {
            RecordsArr[i].Record_Tracks[k].Record_Name = NULL;
            RecordsArr[i].Record_Tracks[k].Track_Name = NULL;
            RecordsArr[i].Record_Tracks[k].Track_Length = 0;
            RecordsArr[i].Record_Tracks[k].Track_Position = 0;
        }
    }

    for(int i = 0; i < RECSIZE; ++i)
    {
        if(RecordsArr[i].Record_ID == 0) // ADDS TO THE FIRST EMPTY PLACE
        {
            RecordsArr[i].Record_Name = strdup(RecordName);
            RecordsArr[i].Record_Year = RecordYear;
            RecordsArr[i].TracksSize = TracksNumber;
            RecordsArr[i].Record_Category = RecordCategory;
            RecordsArr[i].Record_ID = i+1;        
            RecordsArr[i].Record_Tracks = (track*)malloc(sizeof(track) * TracksNumber);
            break;
        }
    }

    return SUCCESS;
}
records_result Remove_Record(Record *RecordsArr, char *RecordName)
{
    int recordIndex = -1; // IN CASE ID == 0

    for(int i = 0; i < RECSIZE; ++i) // GETS THE INDEX OF THE SELECTED RECORD USING RECORD NAME
    {
        if(strcmp(RecordsArr[i].Record_Name, RecordName) == 0) {
            recordIndex = i;
            break;
        }
    }

    if(recordIndex == -1)
        return RECORD_DOESNT_EXIST;

    free(RecordsArr[recordIndex].Record_Name); // FINALIZE THE ARRAY OF THE CHAR* THAT WAS USED BY STRDUP FUNC
    for(int i = 0; i < RecordsArr[recordIndex].TracksSize; ++i)
    {
        RecordsArr[recordIndex].Record_Tracks[i].Record_Name = NULL;
        RecordsArr[recordIndex].Record_Tracks[i].Track_Length = 0;
        RecordsArr[recordIndex].Record_Tracks[i].Track_Position = 0;
        RecordsArr[recordIndex].Record_Tracks[i].Track_Name = NULL;
    }

    free(RecordsArr[recordIndex].Record_Tracks); // FINALIZING THE ARRAY OF THE TRACKS
    RecordsArr[recordIndex].Record_ID = 0;  

    for(int i = recordIndex + 1; i < RECSIZE; ++i) // REDUCE ARRAY AFTER DELETING OBJECT
    {
        RecordsArr[i-1] = RecordsArr[i];
        RecordsArr[i-1].Record_Tracks = RecordsArr[i].Record_Tracks;
        if(RecordsArr[i-1].Record_Name != NULL)
            RecordsArr[i-1].Record_ID -= 1;
    }

    return SUCCESS;
}

records_result Add_Track_To_Record(Record *RecordsArr, char *RecordName, char *TrackName, int TrackLength, int TrackPosition)
{
    int recordIndex = -1;
    int counter = 0;

    for(int i = 0; i < RECSIZE; ++i)
    {
        if(RecordsArr[i].Record_Name != NULL)
        {
            if(strcmp(RecordsArr[i].Record_Name, RecordName) == 0)
            {
                recordIndex = i;
                break;
            }
        }
    }

    if(recordIndex == -1)
        return RECORD_DOESNT_EXIST;

    for(int i = 0; i < RecordsArr[recordIndex].TracksSize; ++i)
        if(RecordsArr[recordIndex].Record_Tracks[i].Track_Position != 0)
            if(strcmp(RecordsArr[recordIndex].Record_Tracks[i].Track_Name, TrackName) == 0)
                return TRACK_ALREADY_EXISTS;

    if(TrackLength <= 0)
        return INVALID_TRACK_LENGTH;
    if(TrackPosition < 0 || TrackPosition > RecordsArr[recordIndex].TracksSize)
        return INVALID_TRACK_POSITION;

    for(int i = 0; i < RecordsArr[recordIndex].TracksSize; ++i)
        if(RecordsArr[recordIndex].Record_Tracks[i].Record_Name != NULL)
            counter++;

    if(counter == RecordsArr[recordIndex].TracksSize)
        return TRACK_OVERFLOW;

    for(int i = RecordsArr[recordIndex].TracksSize - 1; i >= TrackPosition; i--)
        RecordsArr[recordIndex].Record_Tracks[i+1] = RecordsArr[recordIndex].Record_Tracks[i];

    RecordsArr[recordIndex].Record_Tracks[TrackPosition].Record_Name = strdup(RecordName);
    RecordsArr[recordIndex].Record_Tracks[TrackPosition].Track_Name = strdup(TrackName);
    RecordsArr[recordIndex].Record_Tracks[TrackPosition].Track_Length = TrackLength;
    RecordsArr[recordIndex].Record_Tracks[TrackPosition].Track_Position = TrackPosition;
    RecordsArr[recordIndex].Number_Of_Tracks += 1;

    for(int i = 0; i < RecordsArr[recordIndex].TracksSize; ++i)
        if(RecordsArr[recordIndex].Record_Tracks[i].Track_Name != NULL)
            RecordsArr[recordIndex].Record_Tracks[i].Track_Position = i;

    return SUCCESS;
}

int Compare_Record_Name(const void *a, const void *b) {
    Record *recordA = (Record *)a;
    Record *recordB = (Record *)b;
    return strcmp(recordA->Record_Name, recordB->Record_Name);
}

void Sort_Records_Lexicographically(Record *RecList) 
{
    int numRecords = 0;

    for(int i = 0; i < RECSIZE; ++i)
        if(RecList[i].Record_ID != 0)
            numRecords++;

    qsort(RecList, numRecords, sizeof(Record), Compare_Record_Name);
    
}
records_result Report_Records(Record *RecordsArr, FILE *out, record_category RecordCategory)
{
    int flag = 0;

    if(RecordCategory < 0 || RecordCategory > 8)
        return INVALID_CATEGORY;

    for(int i = 0; i < RECSIZE; ++i)
        if(RecordsArr[i].Record_Category == RecordCategory)
            if(RecordsArr[i].Record_ID != 0)
                flag = 1;

    if(flag == 0 && RecordCategory != 7)
        return NO_RECORDS;

    Sort_Records_Lexicographically(RecordsArr);

    for(int i = 0; i < RECSIZE; ++i)
    {
        if(RecordsArr[i].Record_Name != NULL)
        {
            if(RecordsArr[i].Record_Category == RecordCategory)
            {
                Report_Tracks(RecordsArr, out, RecordsArr[i].Record_Name);
            }
        }
    }
    if(RecordCategory == ALL_CATEGORIES)
    {
        for(int i = 0; i < RECSIZE; ++i)
        {
            if(RecordsArr[i].Record_ID != 0)
            {
                Report_Tracks(RecordsArr, out, RecordsArr[i].Record_Name);
            }
        }
    }
    return SUCCESS;
        
}

records_result Report_Tracks(Record *RecordsArr, FILE *out, char *RecordName)
{
    FILE *fp = out;
    int recordIndex = -1;
    int flag = 0;

    for(int i = 0; i < RECSIZE; ++i)
    {
        if(RecordsArr[i].Record_ID != 0)
        {
            if(strcmp(RecordsArr[i].Record_Name, RecordName) == 0)
            {
                recordIndex = i;
                break;
            }
        }
    }

    if(recordIndex == -1)
        return RECORD_DOESNT_EXIST;

    for(int i = 0; i < RecordsArr[recordIndex].TracksSize; ++i)
        if(RecordsArr[recordIndex].Record_Tracks[i].Record_Name != NULL)
            flag = 1;
    
    if(flag == 0)
        return NO_TRACKS;

    prog2_report_record(fp, RecordsArr[recordIndex].Record_Name, RecordsArr[recordIndex].Record_Year, 
    RecordsArr[recordIndex].Number_Of_Tracks, RecordsArr[recordIndex].Record_Category);
    
    for(int i = 0; i < RecordsArr[recordIndex].TracksSize; ++i)
        if(RecordsArr[recordIndex].Record_Tracks[i].Track_Name != NULL)
            prog2_report_track(fp, RecordsArr[recordIndex].Record_Tracks[i].Track_Name, 
            RecordsArr[recordIndex].Record_Tracks[i].Track_Position, RecordsArr[recordIndex].Record_Tracks[i].Track_Length);

    return SUCCESS;
}

records_result Report_Containing_Records(Record *RecordsArr, FILE *out, char *TrackName)
{
    FILE *fp = out;
    int flag = 0;
    int flag2 = 0;
    int i = 0;

    while(RecordsArr[i].Record_ID != 0)
    {
        for(int k = 0; k < RECSIZE; ++k)
            if(RecordsArr[i].Record_Tracks[k].Record_Name != NULL)
                if(strcmp(RecordsArr[i].Record_Tracks[k].Track_Name, TrackName) == 0)
                    flag = 1;
                    
        ++i;
    }

    for(int i = 0; i < RECSIZE; ++i)
        if(RecordsArr[i].Record_ID != 0)
            flag2 = 1;

    if(flag2 == 0)
        return NO_RECORDS;
    if(flag == 0)
        return TRACK_DOESNT_EXIST;

    while(RecordsArr[i].Record_ID != 0)
    {
        for(int k = 0; k < RECSIZE; ++k)
            if(RecordsArr[i].Record_Tracks[k].Record_Name != NULL)
                if(strcmp(RecordsArr[i].Record_Tracks[k].Track_Name, TrackName) == 0)
                    prog2_report_record(fp,RecordsArr[i].Record_Name, RecordsArr[i].Record_Year, RecordsArr[i].Number_Of_Tracks, RecordsArr[i].Record_Category);
                    
        ++i;
    }

    return SUCCESS;
}
void Report_Errors(records_result res) // REPORTS ERROR USING PROG2 ERROR FUNCTION
{
    FILE *fp;

    // OPEN THE ERRORS.ERR FILE FOR WRITING
    fp = freopen("errors.err", "w", stderr);
    if (fp == NULL) {
        perror("freopen");
        exit(EXIT_FAILURE);
    }

    // CALLS PROG2 REPORT FUNCTION
    prog2_report_error_message(res);

    // CLOSE THE FILE AND RESTORE STANDARD ERROR STREAM 
    fclose(fp);
    freopen("CON", "w", stderr);
}
void Finalize_Records(Record *RecList)
{
    for(int i = 0; i < RECSIZE; ++i) // FINALIZING TRACK RECORDS
        if(RecList[i].Record_Name != NULL)
            free(RecList[i].Record_Tracks);

    for(int i = 0; i < RECSIZE; ++i) // FINALIZING RECORDS ARRAY
        if(RecList[i].Record_Name != NULL)
            free(RecList[i].Record_Name);

    free(RecList);
}
int Execute_Records_DB(int argc,char *input_file_name, char *output_file_name) // EXECUTING THE DB USING argv[2] = input name, argv[4] = output name
{
    Record *RecList = NULL;
    FILE* input_file, *output_file;
    records_result res;
    char line[MAX_LINE_LENGTH];
    char *command, *arg1, *arg2, *arg3, *arg4, *arg5, *arg6, *arg7;

    RecList = Init_Records(RecList);

    // OPEN THE INPUT FILE
    input_file = fopen(input_file_name, "r");
    if (input_file == NULL) {
        fprintf(stderr, "Error: Cannot open input file\n");
        return 1;
    }
    output_file = fopen(output_file_name, "w");
    if(output_file == NULL)
    {
        fprintf(stderr, "Error: Cannot open input file\n");
        return 1;
    }

    // READ COMMANDS FROM THE FILE 
    while (fgets(line, MAX_LINE_LENGTH, input_file) != NULL) {
        // IGNORING EMPTY LINE OR LINES STARTS WITH "#"
        if (strlen(line) <= 1 || line[0] == '#')
            continue;

        //TOKENIZING THE LINES
        command = strtok(line, " \n");
        arg1 = strtok(NULL, " \n");
        arg2 = strtok(NULL, " \n");
        arg3 = strtok(NULL, " \n");
        arg4 = strtok(NULL, " \n");
        arg5 = strtok(NULL, " \n");
        arg6 = strtok(NULL, " \n");
        arg7 = strtok(NULL, " \n");

        // EXECUTE THE COMMAND
        if (strcmp(command, "Add") == 0 && strcmp(arg1, "Record") == 0) 
        {
            if (strcmp(arg5, "Pop") == 0)
                res = Add_Record(RecList, arg2, atoi(arg3), atoi(arg4), POP);
            else if (strcmp(arg5, "Rock") == 0) 
                res = Add_Record(RecList, arg2, atoi(arg3), atoi(arg4), ROCK);
            else if (strcmp(arg5, "Jazz") == 0) 
                res = Add_Record(RecList, arg2, atoi(arg3), atoi(arg4), JAZZ);
            else if (strcmp(arg5, "Classic") == 0) 
                res = Add_Record(RecList, arg2, atoi(arg3), atoi(arg4), CLASSIC);
            else if (strcmp(arg5, "Hip-Hop") == 0) 
                res = Add_Record(RecList, arg2, atoi(arg3), atoi(arg4), HIP_HOP);
            else if (strcmp(arg5, "Blues") == 0) 
                res = Add_Record(RecList, arg2, atoi(arg3), atoi(arg4), BLUES);
            else if (strcmp(arg5, "Country") == 0) 
                res = Add_Record(RecList, arg2, atoi(arg3), atoi(arg4), COUNTRY);
            else if (arg5 == NULL)
                res = Add_Record(RecList, arg2, atoi(arg3), atoi(arg4), ALL_CATEGORIES);
            if (res != SUCCESS)
                Report_Errors(res);
        } 
        else if (strcmp(command, "Remove") == 0 && strcmp(arg1, "Record") == 0) 
        {
            res = Remove_Record(RecList, arg2);
            if (res != SUCCESS) {
                Report_Errors(res);
            }
        } 
        else if (strcmp(command, "Add") == 0 && strcmp(arg1, "Track") == 0 && strcmp(arg2, "To") == 0 && strcmp(arg3, "Record") == 0) 
        {
            res = Add_Track_To_Record(RecList, arg4, arg5, atoi(arg6), atoi(arg7));
            if (res != SUCCESS) {
                Report_Errors(res);
            }
        } 
        else if (strcmp(command, "Report") == 0 && strcmp(arg1, "Records") == 0) 
        {
            if (arg2 == NULL)
                res = Report_Records(RecList, output_file, ALL_CATEGORIES);
            else if (strcmp(arg2, "Rock") == 0) 
                res = Report_Records(RecList, output_file, ROCK);
            else if (strcmp(arg2, "Jazz") == 0) 
                res = Report_Records(RecList, output_file, JAZZ);
            else if (strcmp(arg2, "Classic") == 0) 
                res = Report_Records(RecList, output_file, CLASSIC);
            else if (strcmp(arg2, "Hip Hop") == 0) 
                res = Report_Records(RecList, output_file, HIP_HOP);
            else if (strcmp(arg2, "Blues") == 0) 
                res = Report_Records(RecList, output_file, BLUES);
            else if (strcmp(arg2, "Country") == 0) 
                res = Report_Records(RecList, output_file, COUNTRY);
            else if (strcmp(arg2, "Pop") == 0)
                res = Report_Records(RecList, output_file, POP);

            if(res != 0)
                prog2_report_error_message(res);
        } 
        else if (strcmp(command, "Report") == 0 && strcmp(arg1, "Tracks") == 0) 
        {
            res = Report_Tracks(RecList, output_file, arg2);
            if (res != SUCCESS) {
                prog2_report_error_message(res);
            }
        } 
        else if (strcmp(command, "Report") == 0 && strcmp(arg1, "Containing") == 0 && strcmp(arg2, "Records") == 0) 
        {
            res = Report_Containing_Records(RecList, output_file, arg3);
            if (res != SUCCESS) {
                printf("--%d--", res);
                prog2_report_error_message(res);
            }
        } 
        else 
        {
            fprintf(stderr, "Error: Invalid command\n");
        }
    }

    // CLOSES OPENED FILES

    fclose(input_file);
    fclose(output_file);

    // FINALIZES RECORDS ARRAY
    Finalize_Records(RecList);

    return 0;
}