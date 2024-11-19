# Stock_market_predictor

## Ideas

- Portfolio Manager
    - Provide 10 companies to invest with and obtain the data from one year for each of the companies
    - Given the price at the day, decide how much money from your total assets should be allocated to the stock 
- Investment Manager 
    - Given the volatility of the current volatility of the stock, choose to either keep your money in the stock or sell it

## Strategies
There will be three main strategies:

- Conservative: Approach that sells more often when the volatility is higher
- Neutral: Approach sells and buys an equal amount given a normal range of volatility
- Optimistic: Approach sells less often when the volatility is higher



## Mathematical Formulas


### $Volatility = \sqrt{\frac{\sum_{i=1}^N (r_i - \bar{r})^2}{N-1}}$

Where:
- $sigma$: Volatility (standard deviation of returns)
- $r_i$: Return of the stock in the \(i\)-th period
- $\bar{r}$: Average return over the period
- $N$: Number of periods

## $r_i$ : Return of the stock over the period


## $\bar{r}$ : Average Return of the stock over the period 