/*
	This file is part of ghtml.

    ghtml is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ghtml is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ghtml.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

typedef struct charbuffer {

    char *pointer;               /* the location of the data */
    unsigned long allocated;    /* how much data is allocated for the pointer */
    unsigned long index;        /* read/write index */
    unsigned long length;       /* known string length */
    unsigned long dynamic;      /* if its not dynamic it cannot be resized */
    
} charbuffer;

charbuffer *charbuffer_from_void(void) {
    charbuffer *new_string = calloc(1, sizeof(charbuffer));
    new_string->dynamic = 1;
    return new_string;
}

int charbuffer_free(charbuffer *string) {
    if (string->dynamic) {
        free(string->pointer);  free(string);
        return 1;
    }   return 0;
}

int charbuffer_increase_allocated(register charbuffer *string, unsigned long amount) {
    if (string && string->dynamic) {
        register unsigned long size = (string->allocated + amount);
        string->pointer = realloc(string->pointer, size);
        string->allocated = size;
        return 1;
    } else return 0;
}

int charbuffer_decrease_allocated(register charbuffer *string, unsigned long amount) {
    if (string && string->dynamic && string->allocated && string->allocated >= amount) {
        register unsigned long size = (string->allocated - amount);
        string->pointer = realloc(string->pointer, size);
        string->allocated = size;
        if (string->index >= size) string->index = (size - 1);
        string->pointer[string->index] = '\0';
        string->length = string->index;
        return 1;
    } else return 0;
}


/*
    Internal Procedure charbuffer_right_justify_index
    
    Extend string memory according to index if applicable.
    
    The validity of the string parameter must be validated or trusted by the caller.
    If the memory is dynamic, and index exceeds allocated, the memory will be resized.
    if the memory is not dynamic and index exceeds allocated, index will point to
    the end of the data record.
    
*/  void charbuffer_right_justify_index(charbuffer *string) {
    if (string->index > string->allocated) {
        if (!charbuffer_increase_allocated(string, (string->index - string->allocated)+1)) {
            string->index = string->allocated - 1;
        }
    }
}

char *charbuffer_index_pointer(charbuffer *string) {
    if (string) {
        charbuffer_right_justify_index(string);
        if (string->pointer) return (string->pointer + string->index);
    }
    return NULL;
}

int charbuffer_write_format(charbuffer *string, char *format, ...) {

    /* Modifies: index, length, allocated, pointer, *pointer */
    /* returns number of bytes written (if any) excluding zero terminator */
    
    unsigned long writeable = 0;    
    int written = 0;
    
    va_list args; va_start(args, format);
    
    if (! string ) return 0;
    
    charbuffer_right_justify_index(string);
                
    _charbuffer_write_format_:
    
    writeable = (string->allocated - string->index);
    written = vsnprintf((string->pointer + string->index), writeable, format, args);
    
    if (written >= writeable) {
        if (string->dynamic) {
            charbuffer_increase_allocated(string, (written - writeable) + 1);
            goto _charbuffer_write_format_;
        } else {
            return 0;
        }
    }
    
    string->index += written;
    string->length +=written;
      
    return written;
        
}

/* recognize a char * as a string */
charbuffer *charbuffer_from_static_scan(char *data) {
    
    register unsigned long finalIndex = 0;
    
    register char *fastSource = data;
    
    if ( ! fastSource ) return NULL;
    
    register charbuffer *this_string = calloc(1, sizeof(charbuffer));

    this_string->pointer = fastSource;
    
    while ( fastSource[finalIndex++] )
    /* testing for null whilst increment */;
    
    this_string->allocated = finalIndex--;
        
    this_string->index = finalIndex;
    this_string->length = finalIndex;
    
    return this_string;
    
}

/* allocate a new dynamic string from char pointer; optional truncate/buffer length */
charbuffer *charbuffer_from_static_spec(char *data, unsigned long length) {
    
    if ( ! data ) return NULL;
    
    if (! length ) { while (data[length++]); length--; };
    
    charbuffer *this_string = charbuffer_from_void();
        
    if (! length ) return this_string;
    
    register char *fastSource = data;
    register char *fastDest = calloc(length + 1, 1);
    
    this_string->pointer = fastDest;
    this_string->allocated = length;
    
    while ( (*fastDest++ = *fastSource++) && length-- );
        
    this_string->index = (this_string->allocated++ - length);
    this_string->length = this_string->index;
    
    return this_string;
    
}

unsigned long charbuffer_scan_length(charbuffer *string) {

    if (!string) return 0;
    
    register char *fastSource = string->pointer;
    register unsigned long finalIndex = 0;
    
    while ( finalIndex != string->allocated )
        if (! fastSource[finalIndex++] ) return --finalIndex;
    
    return finalIndex;
    
}

unsigned long charbuffer_scan_index_length(charbuffer *string) {

    if (!string) return 0;
    
    register char *fastSource = (string->pointer + string->index);
    register unsigned long finalIndex = 0;
    
    while ( finalIndex != string->allocated )
        if (! fastSource[finalIndex++] ) return --finalIndex;
    
    return finalIndex;
    
}

#ifdef BUILDCHARBUFFERTEST

    int main(int argc, char *argv[]) {
        charbuffer *string = charbuffer_from_static_spec("Hello ", 0);
        printf("String Data: %s\n", string->pointer);   
        printf("String Length: %ld\n", string->length);   
        printf("String Index: %ld\n", string->index);   
        printf("String Allocated: %ld\n", string->allocated);
        charbuffer_free(string);
    }
    
#endif
