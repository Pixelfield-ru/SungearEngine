# <p align="center"> Pixelfield`s Coding Convention </p>

---

## Preface
### Please strictly adhere to the rules of this coding agreement in all Pixelfield projects.

---

## Main part of convention
1. ### Naming
   1. ### C++ code files
      All files with C++ code must be named in UpperCamelCase.
      Header (.h) and implementation (.cpp) files must be named according to the following rules:
      - If a header file contains structures, then the header file and the implementation file must be named according to the main structure in the meaning of the code in this file.
        #### Example:
        -- code
        ``` c++
        // in header file
        struct Position
        {
            float m_x { };
            float m_y { };
            float m_z { };
            
            void moveTo(const float& x, const float& y, const float& z) noexcept;
        };
        ```
        -- end code
      
        ####
      
        -- files structure
        > -> Position.h\
          -> Position.cpp
         
        -- end files structure
      - If a header file does not contain structures, its name should clearly indicate its purpose.
        #### Example:
        -- code
        ``` c++
        // in header file
        
        enum class TextureType
        {
            // ...
        };
        
        enum class AttachmentType
        {
            // ...
        };
        ```
        -- end code
        
        ####
        
        -- files structure
        > -> DataTypes.h\
          -> DataTypes.cpp

        -- end files structure
   2. ### Namespaces
      All namespaces, whether structs, classes, enums, unions, namespaces, or anything else, must be named according to the UpperCamelCase.
      #### Example:
      -- code
      ``` c++
      struct MyStruct
      {
      };
      
      enum class MyEnum
      {
      };
      
      namespace MyNamespace
      {
      }
      ```
      -- end code
   3. ### Variables
      Each variable must be grammatically correct in English and also have a comprehensive name in terms of meaning.
      - Local variables must be named according to the lowerCamelCase.
        #### Example:
        -- code
        ``` c++
        void myFunc()
        {
            float myLocalVariable { };
        }
        ```
        -- end code
      - Non-static member variables (except events and callbacks) of classes, structures or unions must begin with the prefix m_. The prefix must be followed by the variable name in the lowerCamelCase.
        #### Example:
        -- code
        ``` c++
        struct MyStruct
        {
            float m_myMemberVaribale { };
        };
        ```
        -- end code
      - Static member variables (except events and callbacks) of classes, structures or unions must begin with the prefix s_. The prefix must be followed by the variable name in the lowerCamelCase.
        #### Example:
        -- code
        ``` c++
        struct MyStruct
        {
            static inline float s_myMemberVaribale { };
        };
        ```
        -- end code
      - Member variables denoting an events or callbacks must be named according to lowerCamelCase without any prefixes.
        #### Example:
        -- code
        ``` c++
        struct Screen
        {
            Event<void()> onClicked;
            std::function<void()> onTouched;
        };
        ```
        -- end code
      - Variables in template arguments must be named according to UpperCamelCase.\
        If the type name in the template completely matches the possible name of the variable, then the letter T must be at the beginning of the name.
        #### Example:
        -- code
        ``` c++
        enum class FormatType
        {
        
        };
        
        template<typename SomeType, size_t MyUInt, FormatType TFromatType>
        struct MyStruct
        {
            
        };
        ```
        -- end code
      - Macros must be named according to either lower_snake_case or UPPER_SNAKE_CASE.
        #### Example:
        -- code
        ``` c++
        #define MY_MACRO_0
        #define my_macro_1
        ```
        -- end code
      - constexpr variables, as well as uses and typedefs, must be named according to lower_snake_case.
        #### Example:
        -- code
        ``` c++
        template<typename T>
        struct MyStruct
        {
            using my_type = T;
            typedef T my_type_alias;
            static constexpr inline std::size_t num_dimensions = 3;
        };
        ```
        -- end code
   4. ### Functions
      All functions must be named according to the lowerCamelCase
      #### Example:
      -- code
        ``` c++
        void myFunc()
        {
            // ...
        }
        ```
      -- end code
   5. ### Abbreviations
      If an abbreviation appears at the beginning of a variable or function name, all letters of the abbreviation are written in lowercase.
      #### Example:
      -- code
      ``` c++
      struct AABB
      {
          AABB* m_aabbMember { };
      }
      
      void aabbFunc()
      {
          // ...
      }
      
      ```
      -- end code
      In any other place in the name of a variable or function, all letters of the abbreviation are written in capital letters.
      #### Example:
      -- code
      ``` c++
      struct AABB
      {
          AABB* m_parentAABB { };
      }
      
      void resizeAABB(AABB& aabb)
      {
          // ...
      }
      
      ```
      -- end code
2. ### Spaces
    1. ### Parenthesis
       There should be no spaces after the starting (left paren) and before the ending (right paren) brackets.\
       If there are no arguments, no spaces are included.
       #### Example:
       -- code
       ``` c++
       void doSomething(float a)
       {
           try
           {
           }
           catch(const std::exception& e)
           {
           }
       }
       ```
       -- end code
    2. ### Semicolons
       There should be no space before the semicolons, and there should be one after them.
       #### Example:
       -- code
       ``` c++
       void doSomething()
       {
           for(int i = 0; i < 3; ++i)
           {
           }
       }
       ```
       -- end code
   3. ### Operators
      There must be one space before and after the use of subtraction, addition, division, multiplication, assignment, equality and inequality operators.
      #### Example:
      -- code
      ``` c++
      void doSomething()
      {
          int a = 3;
          int b = 5;
          int c = a - b + (a * a);
      }
      ```
      -- end code
   4. ### Square brackets
      There must be no spaces after the opening square bracket and before the closing square bracket.\
      If there are no arguments, no spaces are included.
      #### Example:
      -- code
      ``` c++
      void doSomething()
      {
          auto [a, b] = std::make_tuple(1, 2);
      }
      ```
      -- end code
   5. ### Commas
      There should be no spaces before the commas (for multiple listings), and there should be one space after each comma.\
      If there are no arguments, no spaces are included.
      #### Example:
      -- code
      ``` c++
      void doSomething(float a, float b)
      {
          
      }
      ```
      -- end code
   6. ### Triangular brackets
      There must be no spaces after the opening square bracket and before the closing square bracket.\
      There should be no spaces before the commas (for multiple listings), and there should be one space after each comma.\
      If there are no arguments, no spaces are included.
      #### Example:
      -- code
      ``` c++
      void doSomething()
      {
          std::unordered_map<int, int> myMap;
      }
      ```
      -- end code
   7. ### Curly braces
      There should be one space before the initial curly brace and before the final curly brace in the initialization expression.\
      If there are no arguments, no spaces are included.
      #### Example:
      -- code
      ``` c++
      void doSomething()
      {
          auto myVar = { 3, 4, 5 };
      }
      ```
      -- end code
      If a multi-line initialization in curly braces is assumed, then the starting curly brace can be either on a new line or on the same line as the initialization.
      #### Example:
      -- code
      ``` c++
      void doSomething()
      {
          auto myVar0 = { 3,
                          4, 
                          5 };
       
          auto myVar1 = 
          { 
              3,
              4, 
              5
          };
      }
      ```
      -- end code
      The beginning brace of a lambda expression can be placed either on a new line or on the same line as the declaration of the lambda expression.\
      When declaring a lambda expression on a single line, there must be one space after the beginning brace and before the ending brace.\
      When declaring a lambda expression, there must be a space before the starting curly brace.
      #### Example:
      -- code
      ``` c++
      void doSomething()
      {
          auto myLambda0 = []() { return 2 + 2; };
      
          auto myLambda1 = []() 
          { 
              return 2 + 2; 
          };
      
          auto myLambda2 = []() { 
              return 2 + 2; 
          };
      }
      ```
      -- end code
      The curly braces of the body of functions, namespaces, unions, structures, and classes must start on a new line.
      #### Example:
      -- code
      ``` c++
      namespace MyNamespace
      {
          struct MyString
          {
              union IntenalBuffer
              {
              }
          }
      
          void doSomething()
          {
              
          }
      }
      ```
      -- end code
   8. ### Calls to namespaces/static members
      Spaces are not used before or after references to namespaces, structures, unions, classes, and static members.
      #### Example:
      -- code
      ``` c++
      void doSomething()
      {
          Core::Internal::doSomethingOther();
      }
      ```
      -- end code
   9. ### Colons
      There must be a space before and after the colons.
      #### Example:
      -- code
      ``` c++
      void doSomething()
      {
          std::vector<int> values { 1, 2, 3 };
      
          for(const auto& v : values)
          {
          }
      }
      ```
      -- end code
3. ### Members placement
   All public members must be placed at the top of the structure, all private members must be placed at the end of the structure.\
   Each private or public block has the following order of placement of members:
   1. Declaration sub-namespaces.
   2. Declaration of member variables, uses, typedefs.
   3. Declaration of functions.
   #### Example:
   -- code
   ``` c++
   struct MyStruct
   {
       struct MySubStruct
       {
       
       }
       
       using type = float;
       
       static constexpr int my_constexpr_var = 4;
       
       float m_myVar = 3.14f;
   
       void doSomething()
       {
   
       }
   
   private:
       float m_myInternalVariable = 9.8f;
       
       void doSomethingInternal()
       {
   
       }
   };
   ```
   -- end code
4. ### Includes
   When including a file from a third-party project (for example, a library), use triangular brackets.\
   When including a file from the current project, use quotation marks.
   #### Example:
   -- code
      ``` c++
      #include <SomeLibrary/Test.h>
      #include "MyProject/Test.h"
      ```
   -- end code
5. ### Good practices
   1. Do not use 'using namespace' expression.
   2. Use smart pointers if you expect multiple pointers to an object or if you expect the concept of RAII. Also watch out for circular references when using smart pointers.
   3. Try to avoid iterating over std::unordered_map, std::map or any other maps.
      