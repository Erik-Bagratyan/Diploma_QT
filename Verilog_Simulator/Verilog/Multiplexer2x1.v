module Multiplexer2x1(a, b, cin, out);

  input a, b, cin;
  wire w1, w2, w3;

  output out;

  and a1 (w1, a, cin);
  not nt1 (w2, cin);
  and a2 (w3, w2, b);
  or o1 (out, w1, w3);
  
endmodule
