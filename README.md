ants
====
simple ants swarm simulation with opengl


build the project
--------------------------------------

clone the project by running
```bash
git clone git://github.com/etixsoftware/ants.git
```

install glut (OpenGL Utility Toolkit) if you use apt-get run following command
```bash
sudo apt-get install freeglut3 freeglut3-dev
```

build the project
```bash
cd ants && make all
```

run simulation with
```bash
./ants
```
control the simulation
--------------------------------------

``` 
(c) config				
    (a) ants
        (n) count
            (number number of ants)
            (ENTER) -> return to config
    (f) food
        (n) count
            (number number of food)
            (ENTER) -> return to config
    (s) size of field
        (x) width
            (width)
            (ENTER) -> return to config
        (y) height
            (height)
            (ENTER) -> return to config
    (p) period
        (period)
        (ENTER)
(w) faster -10ms
(s) slower +10ms
(e) very fast
(q) quit
(h) halt on/off
(p) play
(r) reset
(d) debug on/off
```
