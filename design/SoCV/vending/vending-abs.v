/* 
   Source   : vending/vending.v
   Synopsis : Written by Cheng-Yin Wu for SoCV Assignments
   Date     : 2013/02/20
   Version  : 1.0
   revised  : Simplified by Chu Chen-Kai @ 2013/02/26
*/

// Service Types
`define SERVICE_OFF     2'b00
`define SERVICE_ON      2'b01
`define SERVICE_BUSY    2'b10
// Coin Types

`define NTD_10          2'b01

// Coin Values

`define VALUE_NTD_10    8'd10

// Item Types
`define ITEM_NONE       2'b00
`define ITEM_A          2'b01

// Item Costs
`define COST_A          8'd8


module vendingMachine(
   // Property Output Ports
  z0,
  z1, 
  z2,
   // General I/O Ports
   clk,
   reset,
   // Input Ports
   coinInNTD_10,
   itemTypeIn
);

// Property Output Ports
output z0, z1, z2; 

// General I/O Ports
input  clk;
input  reset;
// Input Ports

input  [1:0] coinInNTD_10;     // input number of NTD_10

input  [1:0] itemTypeIn;       // type of an item
// Output Ports



reg    [2:0] coinOutNTD_10;    // output number of NTD_10

reg    [1:0] itemTypeOut;      // type of an item
reg    [1:0] serviceTypeOut;   // type of the service


reg    [2:0] countNTD_10;      // number of NTD_10


reg    [7:0] inputValue;       // total amount of input money
reg    [7:0] serviceValue;     // total amount of service money
reg    [1:0] serviceCoinType;  // type of the coin for the service
reg          exchangeReady;    // ready for exchange in BUSY
reg          initialized;      // initialized or not (i.e. reset)
reg    [7:0] initValue;        // initial value in vending machine


reg    [2:0] coinOutNTD_10_w;  // output number of NTD_10

reg    [1:0] itemTypeOut_w;    // type of an item
reg    [1:0] serviceTypeOut_w; // type of the service


reg    [2:0] countNTD_10_w;    // number of NTD_10


reg    [7:0] inputValue_w;     // total amount of input money
reg    [7:0] serviceValue_w;   // total amount of service money
reg    [1:0] serviceCoinType_w;// type of the coin for the service
reg          exchangeReady_w;  // ready for exchange in BUSY
reg    [7:0] initValue_w;      // initial value in vending machine

// reg          limit, limit_w;

wire   [7:0] outExchange;      // the output exchange amount, for verification
wire   [7:0] outItemValue;
wire   [7:0] machineValue;

// Property Logic
/***** whether the change is right *****/


assign z0 = initialized && (serviceTypeOut == `SERVICE_ON) && (itemTypeOut != `ITEM_NONE);
assign z1 = initialized && (serviceTypeOut == `SERVICE_OFF) && (inputValue != (outExchange + outItemValue)); 
assign z2 = initialized && (serviceTypeOut == `SERVICE_BUSY) && (serviceTypeOut == `SERVICE_ON);


assign outExchange = (`VALUE_NTD_10 * {5'd0, coinOutNTD_10});


assign outItemValue = (itemTypeOut == `ITEM_A) ? `COST_A : 8'd0;


assign machineValue = (`VALUE_NTD_10 * {6'd0, countNTD_10});

always @ (*) begin
  
  coinOutNTD_10_w   = coinOutNTD_10;
  
  itemTypeOut_w     = itemTypeOut;
  serviceTypeOut_w  = serviceTypeOut;
 
  countNTD_10_w     = countNTD_10;
 
  inputValue_w      = inputValue;
  serviceValue_w    = serviceValue;
  serviceCoinType_w = serviceCoinType;
  exchangeReady_w   = exchangeReady;
 

  case (serviceTypeOut)
    `SERVICE_ON   : begin
      if (itemTypeIn != `ITEM_NONE) begin  // valid request
        
        coinOutNTD_10_w   = 3'd0;
       

        itemTypeOut_w     = itemTypeIn;
        serviceTypeOut_w  = `SERVICE_BUSY;
       
        countNTD_10_w     = (({1'b0, countNTD_10} + {2'd0, coinInNTD_10}) >= {1'b0, 3'b111}) ? 
                             3'b111 : (countNTD_10 + {1'b0, coinInNTD_10});
        
        inputValue_w        = (`VALUE_NTD_10 * {6'd0, coinInNTD_10});

        
        serviceValue_w      = (itemTypeIn == `ITEM_A) ? `COST_A : 8'd0;

        
        serviceCoinType_w = `NTD_10;
        
      end
    end
    `SERVICE_OFF  : begin  // Output change and item
      
      coinOutNTD_10_w   = 3'd0;
      
      itemTypeOut_w     = `ITEM_NONE;
      serviceTypeOut_w  = `SERVICE_ON;
      
    end
    default       : begin  // check change for the item
      if (!exchangeReady) begin
        if (inputValue < serviceValue) begin  // too few money for the item
          serviceValue_w  = inputValue;
          itemTypeOut_w   = `ITEM_NONE;
          exchangeReady_w = 1'b1;
        end else begin
          serviceValue_w  = inputValue - serviceValue;
          exchangeReady_w = 1'b1;
        end
      end else begin
        case (serviceCoinType)
          
          `NTD_10 : begin
            if (serviceValue >= `VALUE_NTD_10) begin
              if (countNTD_10 == 3'd0) begin
                
                serviceValue_w    = inputValue;
                itemTypeOut_w     = `ITEM_NONE;
                serviceCoinType_w = `NTD_10;
                countNTD_10_w     = countNTD_10 + coinOutNTD_10;
                coinOutNTD_10_w   = 3'd0;
                serviceTypeOut_w  = `SERVICE_BUSY;
              end else begin
                coinOutNTD_10_w = coinOutNTD_10 + 3'd1;
                countNTD_10_w   = countNTD_10 - 3'd1;
                serviceValue_w  = serviceValue - `VALUE_NTD_10;
              end
            end else begin
              
              serviceTypeOut_w = `SERVICE_OFF;
            end
          end
          
       
        endcase
      end
    end
  endcase
end

always @ (posedge clk) begin
   if (!reset) begin
      
      coinOutNTD_10     <= 3'd0;
    
      itemTypeOut       <= `ITEM_NONE;
      serviceTypeOut    <= `SERVICE_ON;
      
      countNTD_10       <= 3'd2;
      
      inputValue        <= 8'd0;
      serviceValue      <= 8'd0;
      serviceCoinType   <= `NTD_10;
      exchangeReady     <= 1'b0;
      initialized       <= 1'b1;
      
   end
   else begin
      
      coinOutNTD_10     <= coinOutNTD_10_w;
      
      itemTypeOut       <= itemTypeOut_w;
      serviceTypeOut    <= serviceTypeOut_w;
      
      countNTD_10       <= countNTD_10_w;
      
      inputValue        <= inputValue_w;
      serviceValue      <= serviceValue_w;
      serviceCoinType   <= serviceCoinType_w;
      exchangeReady     <= exchangeReady_w;
      initialized       <= initialized;
      
   end
end

endmodule
