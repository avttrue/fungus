# Rules  

##  

<img src="qrc:/resources/img/rule.svg" height="32"/>

*The field life rule has properties and activities list.*

## Properties

* **Name** - *just the name of the rule, should not be empty*
* **CurseTime** - *the time during which the cell will remain cursed (always:* **-1** *)*

## Activity

*Activities list is the main part of the rule.*

*The activity structure is as follows:*

> set **Activity_Type** if cell is **State** and **Activity_Target** is **{ [Target_State] [Operand] [Operator] [Value] }** and than **ABORT_FLAG**

*For example the "Conway's LIFE game" rule looks like this:*

> set **BIRTH** if cell is **DEAD** and **NEAR** is **{ ALIVE COUNT EQUAL 3 }** and than **ABORT**

> set **DEATH** if cell is **ALIVE** and **NEAR** is **{ ALIVE COUNT LESS 2 }** and than **ABORT**

> set **DEATH** if cell is **ALIVE** and **NEAR** is **{ ALIVE COUNT MORE 3 }** and than **ABORT**

*Activities are applied one by one from the list. The first completed activity interrupts the rule on the current field tick if the ABORT condition is enabled.*

*The activity checks the* **current** *state of the cell, and the result is written to the* **new** *state of the cell.* 

*When the entire rule is completed for all cells, the* **new** *cell states become* **current**. 

*This completes one tick of the field.*

### Activity type

* **BIRTH** - *sets cell state to* **ALIVE**

* **DEATH** - *sets cell state to* **DEAD**

* **CURCE** - *sets cell state to* **CURSED**

* **NOTHING** - *do nothing (* **experimental** *)*

* **UP_AGE** - *to increase age of cell by* **1** 

* **DOWN_AGE** - *to decrease age of cell by* **1** *, but not less than to* **0**

* **INVERT** - *to invert cell state (* **experimental** *)*

* **TRAIT** - *set TRAIT to the cell*

* **WO_TRAIT** - *reset TRAIT at the cell (***w***ith***o***ut trait)* 

### State and Target state

*This is just cell's own state or target cell(s) state.*

### Activity target

* **SELF** - *the cell's own properties, the* **Target_State** *will be ignored*

* **NEAR** - *properties of neighboring cells*

* **GROUP** - *the cell's own properties and properties of neighboring cells*

* **TOP** - *properties of top cell*

* **TOPRIGHT** - *properties of top-right cell*

* **RIGHT** - *properties of right cell*

* **BOTTOMRIGHT** - *properties of bottom-right cell*

* **BOTTOM** - *properties of bottom cell*

* **BOTTOMLEFT** - *properties of bottom-left cell*

* **LEFT** - *properties of left cell*

* **TOPLEFT** - *properties of top-left cell*

### Activity operand

* **COUNT** - *cells count (NEAR and GROUP targets, for SELF - always* **1** *)*

* **AGE** - *cell age or total age of cells (SELF, NEAR, GROUP targets)* 

* **GEN** - *value of cell generation or total generations of cells (SELF, NEAR, GROUP targets)*

* **TRAIT** - *total cells with TRAIT (SELF, NEAR, GROUP targets)*

### Activity operator

* **EQUAL** - *this is* **==** *operator* 

* **MORE** - *this is* **>** *operator* 

* **LESS** - *this is* **<** *operator* 

* **NOT** - *this is* **!=** *operator* 

* **IS_DIV** - *if* **Operand** *or* **Value** *is equal to* **0** *will be returned* **FALSE** *, else will be returned* **operand** *divisibility by* **value**.

### Activity value

*This is just integer value for apply an* **Operator** *to an* **Operand**.

### Abort flag

*This flag means - to interrupt or continue executing the rule after this activity is triggered.*


##  

##  

 ---
