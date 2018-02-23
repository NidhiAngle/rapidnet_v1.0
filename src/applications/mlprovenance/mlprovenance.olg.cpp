/* Copyright (C) 1991-2016 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses Unicode 8.0.0.  Version 8.0 of the Unicode Standard is
   synchronized with ISO/IEC 10646:2014, plus Amendment 1 (published
   2015-05-15).  */
/* We do not support C11 <threads.h>.  */
materialize(image,infinity,infinity,keys(1,2)).
materialize(prediction,infinity,infinity,keys(1,2,3)).
materialize(associationrule,infinity,infinity, keys(1,2,3)).
materialize(result,infinity, infinity, keys(1,2,3,4)).
r1 pList(@Nd,Id, List) :- image(@Nd,Id, Img), List:=f_predictimage(Img).
r2 pIterate(@Nd,Id, N, List) :- pList(@Nd,Id, List), N:=1.
r3 pIterate(@Nd,Id, N, List) :- pIterate(@Nd,Id, N1,List), N1< f_size(List), N:=N1+1.
r4 prediction(@Nd,Id,Item,Prob) :- pIterate(@Nd,Id, N, List), N <= f_size(List), AnotherList:=f_item(List, N), Item:=f_item(AnotherList, 1), Prob:=f_item(AnotherList,2).
identifiedPattern(@Nd,Id,Item, Rule, Prob) :- associationrule(@Nd,Item, Rule, Prob1), prediction(@Nd,Id, Item, Prob2), Prob:= Prob1.
r6 cList(@Nd,Id,List,Rule,Prob) :- identifiedPattern(@Nd, Id, Item, Rule, Prob), List:=f_classify(Rule).
r7 cIterate(@Nd,Id, N, List,Rule,Prob) :- cList(@Nd,Id, List,Rule,Prob), N:=1.
r8 cIterate(@Nd,Id, N, List,Rule,Prob) :- cIterate(@Nd,Id, N1,List,Rule,Prob), N1< f_size(List), N:=N1+1.
r9 result(@Nd,Id,Rule,Person,Prob) :- cIterate(@Nd,Id, N, List,Rule,Prob1), N <= f_size(List), AnotherList:=f_item(List, N), Person:=f_item(AnotherList, 1), Prob2:=f_item(AnotherList,2), Prob:=Prob1.
