// Copyright 2017 The NXT Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "wire/TerribleCommandBuffer.h"

namespace nxt { namespace wire {

    TerribleCommandBuffer::TerribleCommandBuffer() {
    }

    TerribleCommandBuffer::TerribleCommandBuffer(CommandHandler* handler) : mHandler(handler) {
    }

    void TerribleCommandBuffer::SetHandler(CommandHandler* handler) {
        mHandler = handler;
    }

    void* TerribleCommandBuffer::GetCmdSpace(size_t size) {
        // TODO(kainino@chromium.org): Should we early-out if size is 0?
        //   (Here and/or in the caller?) It might be good to make the wire receiver get a nullptr
        //   instead of pointer to zero-sized allocation in mBuffer.

        if (size > sizeof(mBuffer)) {
            return nullptr;
        }

        char* result = &mBuffer[mOffset];
        mOffset += size;

        if (mOffset > sizeof(mBuffer)) {
            Flush();
            return GetCmdSpace(size);
        }

        return result;
    }

    void TerribleCommandBuffer::Flush() {
        mHandler->HandleCommands(mBuffer, mOffset);
        mOffset = 0;
    }

}}  // namespace nxt::wire
