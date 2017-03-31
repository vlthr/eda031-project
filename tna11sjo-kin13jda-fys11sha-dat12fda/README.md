# building
Navigate to the root folder `/simon-jonas-valthor-fredrik`
Simply run `make` or `make all` to generate the executables within the source folder. 
Then run `make install` and the files will be copied corretly to the `/bin` folder.

# Running the program

The program uses sqlite3 as a database manager so it's important that you have installed this.
If you don't, run:

`sudo apt-get install sqlite3 libsqlite3-dev`

Now, to run start the server type (from the bin folder):
`./servermain <portnumber> <sql|in-memory>`

Where the last option simply chooses which database type to use.

Example, assuming CWD in /bin:
`./servermain 9999 sql`
will start the sql-version of the server
`./servermain 9999 in-memory`
on the other hand will, you guessed it, start the in-memory version of the server.

now you can run the client with:
`./clientmain localhost 9999` 

# Reset Database

The Database.db file is located in /src and can be reset by entering the following command sequence (while in /src):

`rm Databas.db`
`sqlite3 Database.db < initdb.sql`

which will yield a new, empty Database.db file that can be copied via `make install`


