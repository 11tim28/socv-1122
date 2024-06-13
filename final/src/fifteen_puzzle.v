
`define MOTION_UP       2'b00
`define MOTION_RIGHT    2'b01
`define MOTION_DOWN     2'b10
`define MOTION_LEFT     2'b11

module fifteen_puzzle(
    clk,
    rst,
    motion,
    // state,
    p
);
input   clk;
input   rst;
input   [1:0]   motion;
// output  [7:0]   state;
output          p;

reg     [63:0]   state_r;
reg     [63:0]   state_w;
reg     [3:0]   zero_pos_r;
reg     [3:0]   zero_pos_w;

// assign  state   =   state_r;
assign  p       =   (state_r == 64'h123456789abcdef0) ? 1'b1 : 1'b0;

always @(*) begin
    state_w     =   state_r;
    zero_pos_w  =   zero_pos_r;

    case(zero_pos_r)
        4'd0: begin
            case(motion)
                `MOTION_LEFT: begin
                    state_w     =   {state_r[63:8], state_r[3:0], state_r[7:4]};
                    zero_pos_w  =   4'd1;
                end
                `MOTION_UP: begin
                    state_w     =   {state_r[63:20], state_r[3:0], state_r[15:4], state_r[19:16]};
                    zero_pos_w  =   4'd4;
                end
                default: begin
                    state_w     =   state_r;
                    zero_pos_w  =   zero_pos_r;
                end
            endcase
        end

        4'd1: begin
            case(motion)
                `MOTION_LEFT: begin
                    state_w     =   {state_r[63:12], state_r[7:4], state_r[11:8], state_r[3:0]};
                    zero_pos_w  =   4'd2;
                end
                `MOTION_UP: begin
                    state_w     =   {state_r[63:24], state_r[7:4], state_r[19:8], state_r[23:20], state_r[3:0]};
                    zero_pos_w  =   4'd5;
                end
                `MOTION_RIGHT: begin
                    state_w     =   {state_r[63:8], state_r[3:0], state_r[7:4]};
                    zero_pos_w  =   4'd0;
                end
                default: begin
                    state_w     =   state_r;
                    zero_pos_w  =   zero_pos_r;
                end
            endcase
        end

        4'd2: begin
            case(motion)
                `MOTION_LEFT: begin
                    state_w     =   {state_r[63:16], state_r[11:8], state_r[15:12], state_r[7:0]};
                    zero_pos_w  =   4'd3;
                end
                `MOTION_UP: begin
                    state_w     =   {state_r[63:28], state_r[11:8], state_r[23:12], state_r[27:24], state_r[7:0]};
                    zero_pos_w  =   4'd6;
                end
                `MOTION_RIGHT: begin
                    state_w     =   {state_r[63:12], state_r[7:4], state_r[11:8], state_r[3:0]};
                    zero_pos_w  =   4'd1;
                end
                default: begin
                    state_w     =   state_r;
                    zero_pos_w  =   zero_pos_r;
                end
            endcase
        end

        4'd3: begin
            case(motion)
                `MOTION_UP: begin
                    state_w     =   {state_r[63:32], state_r[15:12], state_r[27:16], state_r[31:28], state_r[11:0]};
                    zero_pos_w  =   4'd7;
                end
                `MOTION_RIGHT: begin
                    state_w     =   {state_r[63:16], state_r[11:8], state_r[15:12], state_r[7:0]};
                    zero_pos_w  =   4'd2;
                end
                default: begin
                    state_w     =   state_r;
                    zero_pos_w  =   zero_pos_r;
                end
            endcase
        end

        4'd4: begin
            case(motion)
                `MOTION_LEFT: begin
                    state_w     =   {state_r[63:24], state_r[19:16], state_r[23:20], state_r[15:0]};
                    zero_pos_w  =   4'd5;
                end
                `MOTION_UP: begin
                    state_w     =   {state_r[63:36], state_r[19:16], state_r[31:20], state_r[35:32], state_r[15:0]};
                    zero_pos_w  =   4'd8;
                end
                `MOTION_DOWN: begin
                    state_w     =   {state_r[63:20], state_r[3:0], state_r[15:4], state_r[19:16]};
                    zero_pos_w  =   4'd0;
                end
                default: begin
                    state_w     =   state_r;
                    zero_pos_w  =   zero_pos_r;
                end
            endcase
        end

        4'd5: begin
            case(motion)
                `MOTION_LEFT: begin
                    state_w     =   {state_r[63:28], state_r[23:20], state_r[27:24], state_r[19:0]};
                    zero_pos_w  =   4'd6;
                end
                `MOTION_UP: begin
                    state_w     =   {state_r[63:40], state_r[23:20], state_r[35:24], state_r[39:36], state_r[19:0]};
                    zero_pos_w  =   4'd9;
                end
                `MOTION_DOWN: begin
                    state_w     =   {state_r[63:24], state_r[7:4], state_r[19:8], state_r[23:20], state_r[3:0]};
                    zero_pos_w  =   4'd1;
                end
                `MOTION_RIGHT: begin
                    state_w     =   {state_r[63:24], state_r[19:16], state_r[23:20], state_r[15:0]};
                    zero_pos_w  =   4'd4;
                end
                default: begin
                    state_w     =   state_r;
                    zero_pos_w  =   zero_pos_r;
                end
            endcase
        end

        4'd6: begin
            case(motion)
                `MOTION_LEFT: begin
                    state_w     =   {state_r[63:32], state_r[27:24], state_r[31:28], state_r[23:0]};
                    zero_pos_w  =   4'd7;
                end
                `MOTION_UP: begin
                    state_w     =   {state_r[63:44], state_r[27:24], state_r[39:28], state_r[43:40], state_r[23:0]};
                    zero_pos_w  =   4'd10;
                end
                `MOTION_DOWN: begin
                    state_w     =   {state_r[63:28], state_r[11:8], state_r[23:12], state_r[27:24], state_r[7:0]};
                    zero_pos_w  =   4'd2;
                end
                `MOTION_RIGHT: begin
                    state_w     =   {state_r[63:28], state_r[23:20], state_r[27:24], state_r[19:0]};
                    zero_pos_w  =   4'd5;
                end
                default: begin
                    state_w     =   state_r;
                    zero_pos_w  =   zero_pos_r;
                end
            endcase
        end

        4'd7: begin
            case(motion)
                `MOTION_UP: begin
                    state_w     =   {state_r[63:48], state_r[31:28], state_r[43:32], state_r[47:44], state_r[27:0]};
                    zero_pos_w  =   4'd11;
                end
                `MOTION_DOWN: begin
                    state_w     =   {state_r[63:32], state_r[15:12], state_r[27:16], state_r[31:28], state_r[11:0]};
                    zero_pos_w  =   4'd3;
                end
                `MOTION_RIGHT: begin
                    state_w     =   {state_r[63:32], state_r[27:24], state_r[31:28], state_r[23:0]};
                    zero_pos_w  =   4'd6;
                end
                default: begin
                    state_w     =   state_r;
                    zero_pos_w  =   zero_pos_r;
                end
            endcase
        end

        4'd8: begin
            case(motion)
                `MOTION_LEFT: begin
                    state_w     =   {state_r[63:40], state_r[35:32], state_r[39:36], state_r[31:0]};
                    zero_pos_w  =   4'd9;
                end
                `MOTION_UP: begin
                    state_w     =   {state_r[63:52], state_r[35:32], state_r[47:36], state_r[51:48], state_r[31:0]};
                    zero_pos_w  =   4'd12;
                end
                `MOTION_DOWN: begin
                    state_w     =   {state_r[63:36], state_r[19:16], state_r[31:20], state_r[35:32], state_r[15:0]};
                    zero_pos_w  =   4'd4;
                end
                default: begin
                    state_w     =   state_r;
                    zero_pos_w  =   zero_pos_r;
                end
            endcase
        end

        4'd9: begin
            case(motion)
                `MOTION_LEFT: begin
                    state_w     =   {state_r[63:44], state_r[39:36], state_r[43:40], state_r[35:0]};
                    zero_pos_w  =   4'd10;
                end
                `MOTION_UP: begin
                    state_w     =   {state_r[63:56], state_r[39:36], state_r[51:40], state_r[55:52], state_r[35:0]};
                    zero_pos_w  =   4'd13;
                end
                `MOTION_DOWN: begin
                    state_w     =   {state_r[63:40], state_r[23:20], state_r[35:24], state_r[39:36], state_r[19:0]};
                    zero_pos_w  =   4'd5;
                end
                `MOTION_RIGHT: begin
                    state_w     =   {state_r[63:40], state_r[35:32], state_r[39:36], state_r[31:0]};
                    zero_pos_w  =   4'd8;
                end
                default: begin
                    state_w     =   state_r;
                    zero_pos_w  =   zero_pos_r;
                end
            endcase
        end

        4'd10: begin
            case(motion)
                `MOTION_LEFT: begin
                    state_w     =   {state_r[63:48], state_r[43:40], state_r[47:44], state_r[39:0]};
                    zero_pos_w  =   4'd11;
                end
                `MOTION_UP: begin
                    state_w     =   {state_r[63:60], state_r[43:40], state_r[55:44], state_r[59:56], state_r[39:0]};
                    zero_pos_w  =   4'd14;
                end
                `MOTION_DOWN: begin
                    state_w     =   {state_r[63:44], state_r[27:24], state_r[39:28], state_r[43:40], state_r[23:0]};
                    zero_pos_w  =   4'd6;
                end
                `MOTION_RIGHT: begin
                    state_w     =   {state_r[63:44], state_r[39:36], state_r[43:40], state_r[35:0]};
                    zero_pos_w  =   4'd9;
                end
                default: begin
                    state_w     =   state_r;
                    zero_pos_w  =   zero_pos_r;
                end
            endcase
        end

        4'd11: begin
            case(motion)
                `MOTION_UP: begin
                    state_w     =   {state_r[47:44], state_r[59:48], state_r[63:60], state_r[43:0]};
                    zero_pos_w  =   4'd15;
                end
                `MOTION_DOWN: begin
                    state_w     =   {state_r[63:48], state_r[31:28], state_r[43:32], state_r[47:44], state_r[27:0]};
                    zero_pos_w  =   4'd7;
                end
                `MOTION_RIGHT: begin
                    state_w     =   {state_r[63:48], state_r[43:40], state_r[47:44], state_r[39:0]};
                    zero_pos_w  =   4'd10;
                end
                default: begin
                    state_w     =   state_r;
                    zero_pos_w  =   zero_pos_r;
                end
            endcase
        end

        4'd12: begin
            case(motion)
                `MOTION_LEFT: begin
                    state_w     =   {state_r[63:56], state_r[51:48], state_r[55:52], state_r[47:0]};
                    zero_pos_w  =   4'd13;
                end
                `MOTION_DOWN: begin
                    state_w     =   {state_r[63:52], state_r[35:32], state_r[47:36], state_r[51:48], state_r[31:0]};
                    zero_pos_w  =   4'd8;
                end
                default: begin
                    state_w     =   state_r;
                    zero_pos_w  =   zero_pos_r;
                end
            endcase
        end

        4'd13: begin
            case(motion)
                `MOTION_LEFT: begin
                    state_w     =   {state_r[63:60], state_r[55:52], state_r[59:56], state_r[51:0]};
                    zero_pos_w  =   4'd14;
                end
                `MOTION_DOWN: begin
                    state_w     =   {state_r[63:56], state_r[39:36], state_r[51:40], state_r[55:52], state_r[35:0]};
                    zero_pos_w  =   4'd9;
                end
                `MOTION_RIGHT: begin
                    state_w     =   {state_r[63:56], state_r[51:48], state_r[55:52], state_r[47:0]};
                    zero_pos_w  =   4'd12;
                end
                default: begin
                    state_w     =   state_r;
                    zero_pos_w  =   zero_pos_r;
                end
            endcase
        end

        4'd14: begin
            case(motion)
                `MOTION_LEFT: begin
                    state_w     =   {state_r[59:56], state_r[63:60], state_r[55:0]};
                    zero_pos_w  =   4'd15;
                end
                `MOTION_DOWN: begin
                    state_w     =   {state_r[63:60], state_r[43:40], state_r[55:44], state_r[59:56], state_r[39:0]};
                    zero_pos_w  =   4'd10;
                end
                `MOTION_RIGHT: begin
                    state_w     =   {state_r[63:60], state_r[55:52], state_r[59:56], state_r[51:0]};
                    zero_pos_w  =   4'd13;
                end
                default: begin
                    state_w     =   state_r;
                    zero_pos_w  =   zero_pos_r;
                end
            endcase
        end

        4'd15: begin
            case(motion)
                `MOTION_DOWN: begin
                    state_w     =   {state_r[47:44], state_r[59:48], state_r[63:60], state_r[43:0]};
                    zero_pos_w  =   4'd11;
                end
                `MOTION_RIGHT: begin
                    state_w     =   {state_r[59:56], state_r[63:60], state_r[55:0]};
                    zero_pos_w  =   4'd14;
                end
                default: begin
                    state_w     =   state_r;
                    zero_pos_w  =   zero_pos_r;
                end
            endcase
        end

        default: begin
            state_w     =   state_r;
            zero_pos_w  =   zero_pos_r;
        end

    endcase

end


always @(posedge clk) begin
    if(!rst) begin
        //  SAT case
        // 1 step
        state_r     <=  64'h123456789abcde0f;    
        zero_pos_r  <=  4'd1; 

        // 2 steps
        // state_r     <=  64'h123456789abcd0ef;    
        // zero_pos_r  <=  4'd2;

        // 3 steps
        // state_r     <=  64'h123456789abc0def;    
        // zero_pos_r  <=  4'd3;

        // 4 steps
        // state_r     <=  64'h123456780abc9def;    
        // zero_pos_r  <=  4'd7; 

        // 5 steps
        // state_r     <=  64'h123406785abc9def;    
        // zero_pos_r  <=  4'd11; 

        // 6 steps
        // state_r     <=  64'h023416785abc9def;    
        // zero_pos_r  <=  4'd15; 

        // 7 steps
        // state_r     <=  64'h203416785abc9def;    
        // zero_pos_r  <=  4'd14;

        // 8 steps
        // state_r     <=  64'h230416785abc9def;    
        // zero_pos_r  <=  4'd13;

        // 9 steps
        // state_r     <=  64'h234016785abc9def;    
        // zero_pos_r  <=  4'd12;

        // 10 steps
        // state_r     <=  64'h234816705abc9def;    
        // zero_pos_r  <=  4'd8;


        //  UNSAT case
        // state_r     <=  64'h123456789abfdec0;    
        // zero_pos_r  <=  4'd0;            
    end
    else begin
        state_r     <=  state_w;
        zero_pos_r  <=  zero_pos_w;
    end



end

endmodule