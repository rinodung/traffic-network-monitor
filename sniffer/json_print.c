/*
 * Various outputs for flag of json_object_to_json_string_ext().
 *
 * gcc -Wall -I/usr/include/json-c/ -o json_print json_print.c -ljson-c
 */
#include <json.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main() {
	struct json_object *jobj, *json_object_arr, *medi_array_obj, *obj_temp, *json_parser;
    long currentTime;
	int  i, len;
    char *ip_src, *ip_dest, *str;
	json_object_arr = json_object_new_array();
	for(i = 0; i< 20; i++) {
	    jobj = json_object_new_object();
	    ip_src = "127.0.0.1";
	    ip_dest = "192.168.0.1";
	    currentTime = (long)time(NULL);
    	json_object_object_add(jobj, "ip_src", json_object_new_string(ip_src));
    	json_object_object_add(jobj, "ip_dest", json_object_new_string(ip_dest));
    	json_object_object_add(jobj, "port_src", json_object_new_string("80"));
    	json_object_object_add(jobj, "port_dest", json_object_new_string("90"));
    	json_object_object_add(jobj, "time", json_object_new_int(currentTime));
		json_object_array_add(json_object_arr, jobj);
	}
	
    
    str = json_object_to_json_string_ext(json_object_arr, JSON_C_TO_STRING_PLAIN);
    printf("Using JSON_C_TO_STRING_PLAIN with array:\n");
    printf("%s\n---\n",str);
    json_parser = json_tokener_parse(str);
    len = json_object_array_length(json_parser);
    for (i = 0; i < len; i++) {
      // get the i-th object in json_parser
      medi_array_obj = json_object_array_get_idx(json_parser, i);
       printf("\n----Package %d----\n",i);
      // get the field attribute in the i-th object
      obj_temp  = json_object_object_get(medi_array_obj, "ip_src");
      // print out the name attribute
      printf("ip_src=%s\n", json_object_get_string(obj_temp));
      
      obj_temp  = json_object_object_get(medi_array_obj, "ip_dest");
      // print out the name attribute
      printf("ip_dest=%s\n", json_object_get_string(obj_temp));
      
      obj_temp  = json_object_object_get(medi_array_obj, "port_src");
      // print out the name attribute
      printf("port_src=%s\n", json_object_get_string(obj_temp));
      
      obj_temp  = json_object_object_get(medi_array_obj, "port_dest");
      // print out the name attribute
      printf("port_dest=%s\n", json_object_get_string(obj_temp));
      
       obj_temp  = json_object_object_get(medi_array_obj, "time");
      // print out the name attribute
      printf("time=%s\n", json_object_get_string(obj_temp));
       
    }

	
	
		
	printf("========Using json_tokener_parse:=========\n");
    
    printf("Data from str:\n---\n%s\n---\n", json_object_to_json_string_ext(json_parser, JSON_C_TO_STRING_PRETTY));
	return 0;
}
