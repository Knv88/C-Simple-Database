# C-Simple-Database
A simple c++ database that i have made for my school project.
The main.cpp and clase.h work on a Visual Studio Project,where they need to be put on an empty project, compiled and debuged.
In the debug folder it needs to be created 4 empty folders "Tables" "Select" "Display" "Reports".
The program can run from the CMD and will accept text files with commands as arguments but only if they are in the Debug folder.
 
Examples of commands that the program accepts:(without the first and the last "")

"CREATE TABLE table1_name ((collumn1_name,text,1000,'default_value') , (collumn2_name,integer,1000,'000'))"
                                          ---> first thing we need to create a table ,inside each (..) we have the collumns names, what types of values we are going to 
                                           put on those collums , the maximum byte dimension of each value and the default value. There is no logical limit on how many 
                                           collums or tables we can have.
                                           
"INSERT INTO table1_name VALUES ("value1",1) , ("value2",2) , ("value3",3)" 
                                         ---> each (...) represents a row, and the values inside need to be the exact number of collumns,if a collumn was of type
                                         text ,then the values need to be inside "", otherwise it needs to be a number. We can add as many rows as we want.
                                         
"IMPORT table1_name example.csv"     ---> this is an alternative for INSERT, it can accept cvs's that are in the Debug folder.

"SELECT (collumn1_name) FROM table1_name WHERE collumn2_name = 3"   -->will display the values from collumn 1 from the rows where the values of collumn 2 is 3.
"SELECT ALL FROM table1_name"     
