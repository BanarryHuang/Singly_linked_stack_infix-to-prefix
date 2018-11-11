# Singly_linked_stack_infix-to-prefix
DS_Homework2
程式目的

將課本內3.6節的 eval() 和 postfix() 改寫，以 singly linked lists 取代陣列堆疊實作，

並且變成可以適用在更多種狀況輸入的版本。

主要架構

singly linked lists: struct stack, push(), pop()

getToken(char *symbol, int *n, int x): 跟課本的架構類似，但是多了x來判斷是

infix 轉成 postfix 的部分還是計算 postfix 值的部分。

precedence printToken(precedence d): 把運算子寫入 postequa[postlength]

postfix(): 

1.遇到operand如果沒有負數問題，直接將數字放入 num[length] 中

2.遇到右括號將目前收集的 num[length] 轉成數字存入 postequa[postlength]，

  stack 中運算子全數存入 postequa[postlength] 直到出現左括號停止

3.遇到運算子首先紀錄第一個 token 跟到目前為止的最後一個token

  作為判斷 unary operand 及負數依據

(1)如果運算子是 - 而且有 dupsign (連續出現的 operand) 要調整目前在 stack 的符號

   (負負得正，正負轉成負，乘除負號保留(負數依據))

   +...+、*...*、/.../當作重複輸入(只記一次)

   規則禁止輸入任何 +- 至 */ 前面，*/ 後允許輸入 -

   前面有提到負數的判斷依據，如下：

 	存在 stack 的 token (處理結果), (第一個 token, 最後一個 token)

	+,(+,+):+、+,(+,-):+-、+,(-,+):+、+,(-,-):+、

	-,(+,+):-、-,(+,-):-、-,(-,+):-、-,(-,-):--

   因此只有當+,(+,-)和-,(-,-)會將最後的負號當作負數處理。

(2)非 unary operand 則將目前收集的 num[length](若非空) 轉成數字存入 postequa[postlength]，

  stack 中優先序高的 (isp[top->item] >= icp[token]) 這時也要存入 postequa[postlength]、

  將 dupsign 設為 token，token push 進 stack

最後將跑完迴圈剩下的的數字放入 postequa[postlength]，

並 pop() 完所有運算子放入 postequa[postlength]

eval():

前面經過 postfix() 所得到的單位間皆有空白隔開，所以可以得知斷點(例如數字連續出現的狀況)，

處理其運算，因為做的是浮點數運算，所以用另一個 stack2 來做 pushf() popf() 表示

遇到數字 pushf()，遇到運算子 popf() 出兩樣東西進行運算，再 pushf() 回去

最終 stack2 留下的結果即為 eval() 完的值

時間複雜度：

postfix()：在把 equation 走完的過程中，除了存取數字部分有迴圈之外，大都為判斷式等指令，

	    所以時間複雜度為 O(n)

eval()：在把 postequa 走完的過程中，一樣只有存取數字部分有迴圈之外，大都為判斷式等指令，

	    所以時間複雜度為 O(n)

所以總時間複雜度為 O(n)
