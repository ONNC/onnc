# Add

## Support Spec

* Input `tensor A`
    * input demension == 2 or 4
        * input demension == 2: support 1xk or kxj (k&j are any integer)
        * input demension == 4: 1xCxHxW,H=W, 1st dimension == 1
    * C (1 ~ 2,147,483,647) sign int32
    * H (1 ~ 2,147,483,647) sign int32
    * W (1 ~ 2,147,483,647) sign int32
    * array of values(sign 8 bits)
* Input `tensor B`
    * dimension:CxHxW,H=W=1
    * range:sign 8 bits
    * number = number of `tensor A`'channels
    * `tensor B` size(sign 32 int & >= 1) == CxHxW
* Input `tensor C`
    * dimension: 1xCxHxW = Input `tensor A`


## Testing Spec

