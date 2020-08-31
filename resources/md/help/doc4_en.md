# Field and cells

##  

## Field    

##  

<img src="qrc:/resources/img/field.svg" height="32"/>

 *The field consists of cells, field geometry is closed, like a sphere. Also, the field has a* [rule](doc5_en.md) *of life.*

## Cells    

##  

<img src="qrc:/resources/img/cell.svg" height="32"/> 

 *A cell can have the following states:*

* **DEAD** - *empty cell*
* **ALIVE** - *occupied, live cell*
* **CURSED** - *this cell cannot accept the alive state during the time specified by the rule;*
*when cell switch to this state, the* **GENERATION** *property receive the value* **0**

##  

 *A cell has the following properties:*
 
* **AGE** - *age of a alive cell; when switching from the* **ALIVE** *state to the* **DEAD** *state, the* **AGE** *property receive the value* **0**
* **GENERATION** - *count of generations; when cell switch from* **DEAD** *state to* **ALIVE** *state, the* **GENERATION** *property increases by* **1**
*; when cell switch to* **CURSED** *state, the* **GENERATION** *property receive the value* **0**

##  

##  

 ---
