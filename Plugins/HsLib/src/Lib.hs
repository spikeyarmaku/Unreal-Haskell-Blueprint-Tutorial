{-# LANGUAGE ForeignFunctionInterface #-}

module Lib
    ( hsAdder
    ) where

import Foreign.C.Types

foreign export ccall hsAdder :: CInt -> CInt -> CInt

hsAdder :: CInt -> CInt -> CInt
hsAdder a b = a + b
