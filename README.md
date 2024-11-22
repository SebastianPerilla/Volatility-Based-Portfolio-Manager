# Computer Programming Group Project: Stock Market Investment Manager

## Concept Diagram

![alt text](readme_images/image.png)

## Steps

1. Intro
    1. User is asked about how much money he wants to invest into the selection of stocks
    2. User is asked which strategy he wants to follow:
        - Aggressive
        - Neutral
        - Conservative
2. Volatility Calculation 
    1. Volatility is calculated from the price data for each respective ticker (see below for formula interpretation)
3. Stock Manager + Portfolio Manager
    1. Based on the chosen strategy the stock manager will choose the follwing:
        - If, and how much of the current position in the stock will be sold.
        - If, and how much of the current position in the stock will be added onto the portfolio.
    2. Based on the decision of the stock manager, the portfolio manager will then do the following:
        - Decide how and where to allocate the current available funds to other available stocks within the portfolio.
4. Visualization
    1. Output a log of the weekly portfolio performance with each of the trades/allocations done by the managers.
    2. Show a visualization of the portfolio's value over the time period the user has requested, alongside each individual stock performance.


## Strategies
There will be three main strategies:

- Conservative: Approach that sells more often when the volatility is higher
- Neutral: Approach sells and buys an equal amount given a normal range of volatility
- Optimistic: Approach sells less often when the volatility is higher


## Mathematical Formulas + Examples


### $\text{Volatility}$ = $\sigma$ = $\sqrt{\frac{\sum_{i=1}^N (r_i - \bar{r})^2}{N-1}}$

Where:
- $\sigma$: Volatility (standard deviation of returns)
- $r_i$: Return of the stock in the \(i\)-th period
- $\bar{r}$: Average return over the period
- $N$: Number of periods

## Updating Volatility: Exponentially Weighted Moving Average (EWMA) 

### $\text{Updated Volatility}$ = $\sigma^2_t = (1 - \lambda) r^2_t + \lambda\sigma^2_{t-1}$

Where: 
- $\sigma^2_t$ : Updated Volatility
- $\sigma^2_{t-1}$ : Previous Variance
- $r^t = ln(\frac{P_{new}}{P_{old}})$ : is the log return
- $\lambda$ : is the Decay Factor (Determines how much weight is given to recent returns versus historical volatility.)