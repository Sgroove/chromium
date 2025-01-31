// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package org.chromium.content.browser;

import android.content.Intent;
import android.graphics.drawable.Drawable;
import android.view.View.OnClickListener;

import org.chromium.base.annotations.SuppressFBWarnings;

/**
 * The interface that controls contextual text selection.
 */
@SuppressFBWarnings("UUF_UNUSED_PUBLIC_OR_PROTECTED_FIELD")
public interface ContextSelectionProvider {
    /**
     * The result of the text analysis.
     */
    public static class Result {
        /**
         * The number of characters that the left boundary of the original
         * selection should be moved. Negative number means moving left.
         */
        public int startAdjust;

        /**
         * The number of characters that the right boundary of the original
         * selection should be moved. Negative number means moving left.
         */
        public int endAdjust;

        /**
         * Label for the suggested menu item.
         */
        public CharSequence label;

        /**
         * Icon for the suggested menu item.
         */
        public Drawable icon;

        /**
         * Intent for the suggested menu item.
         */
        public Intent intent;

        /**
         * OnClickListener for the suggested menu item.
         */
        public OnClickListener onClickListener;
    }

    /**
     * The interface that returns the result of the selected text analysis.
     */
    public interface ResultCallback {
        /**
         * The result is delivered with this method.
         */
        void onClassified(Result result);
    }

    /**
     * Sends asynchronous request to obtain the selection, analyze its type and suggest
     * better selection boundaries.
     * @param text  The textual context that encloses the selected text.
     * @param start The start index of the selected text inside the textual context.
     * @param end   The index pointing to the first character that comes after
     *              the selected text inside the textual context.
     */
    public void sendSuggestAndClassifyRequest(CharSequence text, int start, int end);

    /**
     * Sends asynchronous request to obtain the selection and analyze its type.
     * @param text  The textual context that encloses the selected text.
     * @param start The start index of the selected text inside the textual context.
     * @param end   The index pointing to the first character that comes after
     *              the selected text inside the textual context.
     */
    public void sendClassifyRequest(CharSequence text, int start, int end);

    /**
     * Cancel all asynchronous requests.
     */
    public void cancelAllRequests();
}
