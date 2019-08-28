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
    * when `tensor A` of dimension == 4:
        * dimension:CxHxW,H=W=1,C = `tensor A` of channels
        * range:sign 8 bits
        * `tensor B` size(sign 32 int & >= 1) == CxHxW
    * when `tensor A` of dimension == 2:
        * dimension:HxW,H=`tensor A` of H,W=`tensor A` of We
        * range:sign 8 bits
        * `tensor B` size(sign 32 int & >= 1) == HxW
* Output `tensor C`
    * dimension: 1xCxHxW = Input `tensor A`


## Testing Spec

* MatAdd_4D
    * Input `tensor A`:1x4x6x6
    * Input `tensor B`:4x1x1
    * Output `tensor C`:1x4x6x6

* MatAdd_2D
    * Input `tensor A`:6x6
    * Input `tensor B`:6x6
    * Output `tensor C`:6x6
